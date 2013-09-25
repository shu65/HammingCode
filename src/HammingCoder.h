/*
 * HammingCoder.h
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

#ifndef HAMMINGCODER_H_
#define HAMMINGCODER_H_

#include <stdint.h>
#include <tr1/unordered_map>
#include "Matrix.h"

namespace hamming_code {

class HammingCoder {
public:
	HammingCoder();
	HammingCoder(uint8_t alphabet_size, Matrix &generator_matrix,
			Matrix &checker_matrix, Matrix &receiver_matrix);

	virtual ~HammingCoder();

	int Encode(Matrix &data, Matrix *encoded_data);
	int Correct(Matrix &data, Matrix *corrected_data);
	int Decode(Matrix &encoded_data, Matrix *data);

private:
	typedef Matrix Syndrome;
	struct Error {
		size_t error_position;
		Matrix::Value error_value;
	};

	struct SyndromeEqual {
		bool operator()(const Syndrome& syndrome0,
				const Syndrome& syndrome1) const {
			size_t number_colums = syndrome0.GetNumberColums();
			size_t number_rows = syndrome0.GetNumberRows();
			if (number_colums == syndrome1.GetNumberColums()
					&& number_rows == syndrome1.GetNumberRows()) {
				size_t length = number_colums * number_rows;
				const Syndrome::Value *syndrome0_values =
						syndrome0.GetElements();
				const Syndrome::Value *syndrome1_values =
						syndrome1.GetElements();
				for (size_t i = 0; i < length; ++i) {
					if (syndrome0_values[i] != syndrome1_values[i]) {
						return false;
					}
				}
			} else {
				return false;
			}
			return true;
		}
	};

	struct SyndromeHashFunction {
		size_t operator()(const Syndrome& syndrome0) const {
			size_t v = 0;
			size_t number_colums = syndrome0.GetNumberColums();
			size_t number_rows = syndrome0.GetNumberRows();
			size_t length = number_colums * number_rows;
			std::tr1::hash<Syndrome::Value> hash_func;
			const Syndrome::Value *syndrome0_values = syndrome0.GetElements();
			for (size_t i = 0; i < length; ++i) {
				v ^= hash_func(syndrome0_values[i]);
			}
			return v;
		}
	};

	int BuildErrorTable();
	int MatrixModulo(Matrix &matrix, Matrix::Value n);

	typedef std::tr1::unordered_map<Syndrome, Error, SyndromeHashFunction,
			SyndromeEqual> ErrorTable;

	Matrix::Value alphabet_size_;
	Matrix generator_matrix_;
	Matrix checker_matrix_;
	Matrix receiver_matrix_;
	ErrorTable error_table_;

};

} /* namespace hamming_code */
#endif /* HAMMINGCODER_H_ */
