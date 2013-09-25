/*
 * Matrix.cpp
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

#include <assert.h>
#include "Matrix.h"

namespace hamming_code {

int Matrix::Print(std::ostream &ostream, Matrix &matrix) {
	size_t number_rows = matrix.GetNumberRows();
	size_t number_columns = matrix.GetNumberColums();
	Value *values = matrix.GetElements();
	for (size_t row_i = 0; row_i < number_rows; ++row_i) {
		size_t offset = row_i * number_columns;
		for (size_t column_i = 0; column_i < number_columns; ++column_i) {
			ostream << (int) values[offset + column_i] << "\t";
		}
		ostream << std::endl;
	}
	return 0;
}

int Matrix::Multiply(Matrix &matrix0, Matrix &matrix1, Matrix *ret_matrix) {
	if (matrix0.GetNumberColums() == matrix1.GetNumberRows()) {
		size_t number_ret_rows = matrix0.GetNumberRows();
		size_t number_ret_columns = matrix1.GetNumberColums();
		size_t number_matrix0_columns = matrix0.GetNumberColums();
		if (ret_matrix->GetNumberRows() != number_ret_rows
				|| ret_matrix->GetNumberColums() != number_ret_columns) {
			*ret_matrix = Matrix(number_ret_rows, number_ret_columns);
		}
		Value *matrix0_values = matrix0.GetElements();
		Value *matrix1_values = matrix1.GetElements();
		Value *ret_matrix_values = ret_matrix->GetElements();
		for (size_t row_i = 0; row_i < number_ret_rows; ++row_i) {
			size_t offset = row_i * number_ret_columns;
			for (size_t column_i = 0; column_i < number_ret_columns;
					++column_i) {
				Value *ret_value = &ret_matrix_values[offset + column_i];
				*ret_value = 0;
				for (size_t i = 0; i < number_matrix0_columns; ++i) {
					*ret_value += matrix0_values[row_i * number_matrix0_columns
							+ i]
							* matrix1_values[i * number_ret_columns + column_i];
				}
			}
		}
		return 0;
	}
	return 1;
}

Matrix::Matrix() :
		number_rows_(0), number_columns_(0), values_(0) {

}
Matrix::Matrix(size_t number_rows, size_t number_columns) :
		number_rows_(number_rows), number_columns_(number_columns), values_(
				number_rows * number_columns) {
}
Matrix::Matrix(size_t number_rows, size_t number_columns,
		Value *values) :
		number_rows_(number_rows), number_columns_(number_columns), values_(
				values, values + (number_rows*number_columns)) {
}
Matrix::~Matrix() {

}

} /* namespace pchindex */
