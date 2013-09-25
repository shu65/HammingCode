/*
 * HammingCoder.cpp
 *
 *   Copyright (c) 2013, Shuji Suzuki
 *   All rights reserved.
 *
 *   Redistribution and use in source and binary forms, with or without
 *   modification, are permitted provided that the following conditions
 *   are met:
 *
 *   1. Redistributions of source code must retain the above Copyright
 *      notice, this list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above Copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *   3. Neither the name of the authors nor the names of its contributors
 *      may be used to endorse or promote products derived from this
 *      software without specific prior written permission.
 *
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *   PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "HammingCoder.h"
#include "Matrix.h"
#include <cmath>
#include <stdint.h>
#include <assert.h>

namespace hamming_code {

HammingCoder::HammingCoder() :
		alphabet_size_(0) {

}

HammingCoder::HammingCoder(uint8_t alphabet_size, Matrix &generator_matrix,
		Matrix &checker_matrix, Matrix &receiver_matrix) :
		alphabet_size_(alphabet_size), generator_matrix_(generator_matrix), checker_matrix_(
				checker_matrix), receiver_matrix_(receiver_matrix) {
	BuildErrorTable();
}

HammingCoder::~HammingCoder() {

}

int HammingCoder::Encode(Matrix &data, Matrix *encoded_data) {
	Matrix::Multiply(generator_matrix_, data, encoded_data);
	MatrixModulo(*encoded_data, alphabet_size_);
	return 0;
}

int HammingCoder::Correct(Matrix &data, Matrix *corrected_data) {
	Syndrome syndrome;
	Matrix::Multiply(checker_matrix_, data, &syndrome);
	MatrixModulo(syndrome, alphabet_size_);
	bool in_error = false;
	size_t length = syndrome.GetNumberRows();
	Matrix::Value *syndrome_values = syndrome.GetElements();
	for (size_t i = 0; i < length; ++i) {
		if (syndrome_values[i] != 0) {
			in_error = true;
			break;
		}
	}
	*corrected_data = data;
	if (in_error) {
		ErrorTable::iterator find_it = error_table_.find(syndrome);
		assert(find_it != error_table_.end());
		Matrix::Value *error_value = corrected_data->GetElements()
				+ find_it->second.error_position;
		*error_value += find_it->second.error_value;
		if (*error_value >= alphabet_size_) {
			*error_value -= alphabet_size_;
		}
	}
	return 0;
}

int HammingCoder::Decode(Matrix &encoded_data, Matrix *data) {
	Matrix::Print(std::cout, receiver_matrix_);
	return Matrix::Multiply(receiver_matrix_, encoded_data, data);
}

int HammingCoder::BuildErrorTable() {
	Syndrome syndrome;
	size_t word_length = generator_matrix_.GetNumberRows();
	Matrix word(word_length, 1);
	Matrix::Value *word_values = word.GetElements();
	for (size_t i = 0; i < word_length; ++i) {
		Error e;
		e.error_position = i;
		for (Matrix::Value c = 0; c < alphabet_size_; ++c) {
			word_values[i] = c;
			Matrix::Multiply(checker_matrix_, word, &syndrome);
			e.error_value = c;
			error_table_.insert(std::make_pair(syndrome, e));
		}
		word_values[i] = 0;
	}
	return 0;
}

int HammingCoder::MatrixModulo(Matrix &matrix, Matrix::Value n) {
	size_t number_rows = matrix.GetNumberRows();
	size_t number_columns = matrix.GetNumberColums();
	Matrix::Value *values = matrix.GetElements();
	for (size_t row_i = 0; row_i < number_rows; ++row_i) {
		size_t offset = row_i * number_columns;
		for (size_t column_i = 0; column_i < number_columns; ++column_i) {
			values[offset + column_i] %= n;
		}
	}
	return 0;
}

} /* namespace pchindex */
