/*
 * HammingCoderTest.cpp
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

#include <gtest/gtest.h>
#include <stdint.h>
#include <iostream>
#include <fstream>
#include "../src/HammingCoder.h"
#include "../src/Matrix.h"

using namespace std;

class HammingCoderTest: public ::testing::Test {
protected:
	virtual void SetUp() {
		hamming_code::Matrix::Value generator_matrix_data[] = { 1, 1, 0, 1, 1, 0, 1,
				0, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0,
				0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
		hamming_code::Matrix generator_matrix(15, 11, generator_matrix_data);

		hamming_code::Matrix::Value checker_matrix_data[] = { 1, 0, 1, 0, 1, 0, 1,
				0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0,
				1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0,
				0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 };
		hamming_code::Matrix checker_matrix(4, 15, checker_matrix_data);

		hamming_code::Matrix::Value reciver_matrix_data[] = { 0, 0, 1, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
				0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };
		hamming_code::Matrix receiver_matrix(11, 15, reciver_matrix_data);

		hamming_coder_ = hamming_code::HammingCoder(2, generator_matrix,
				checker_matrix, receiver_matrix);
	}

	virtual void TearDown() {
	}
	hamming_code::HammingCoder hamming_coder_;
};

TEST_F(HammingCoderTest, Encode) {
	hamming_code::Matrix::Value pattern_data[] = { 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 };
	hamming_code::Matrix pattern(11, 1, pattern_data);
	hamming_code::Matrix encoded_pattern;
	EXPECT_EQ(0, hamming_coder_.Encode(pattern, &encoded_pattern));

	size_t correct_data_size = 15;
	hamming_code::Matrix::Value correct_data[] = { 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0};
	EXPECT_EQ(1, encoded_pattern.GetNumberColums());
	EXPECT_EQ(correct_data_size,encoded_pattern.GetNumberRows());

	for (size_t i = 0; i < correct_data_size; ++i) {
		EXPECT_EQ(correct_data[i], encoded_pattern.GetElement(0, i));
	}
}

TEST_F(HammingCoderTest, Decode) {
	hamming_code::Matrix::Value pattern_data[] = { 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 };
	size_t pattern_size = 11;
	hamming_code::Matrix pattern(pattern_size, 1, pattern_data);
	hamming_code::Matrix encoded_data(pattern_size, 1);
	EXPECT_EQ(0, hamming_coder_.Encode(pattern, &encoded_data));

	hamming_code::Matrix decoded_data(pattern_size, 1);
	EXPECT_EQ(0, hamming_coder_.Decode(encoded_data, &decoded_data));
	hamming_code::Matrix::Value *data_elements = pattern.GetElements();
	hamming_code::Matrix::Value *decoded_data_elements = decoded_data.GetElements();
	for (size_t i = 0; i < pattern_size; ++i) {
		EXPECT_EQ(data_elements[i], decoded_data_elements[i]);
	}
}

TEST_F(HammingCoderTest, DecodeOneErrorData) {
	hamming_code::Matrix::Value pattern_data[] = { 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0 };
	size_t pattern_size = 11;
	hamming_code::Matrix pattern(pattern_size, 1, pattern_data);
	hamming_code::Matrix encoded_data(pattern_size, 1);
	EXPECT_EQ(0, hamming_coder_.Encode(pattern, &encoded_data));
	encoded_data.SetElement(0, 0, 1);
	hamming_code::Matrix corrected_data;
	EXPECT_EQ(0, hamming_coder_.Correct(encoded_data, &corrected_data));

	hamming_code::Matrix decoded_data(pattern_size, 1);
	EXPECT_EQ(0, hamming_coder_.Decode(corrected_data, &decoded_data));
	hamming_code::Matrix::Value *data_elements = pattern.GetElements();
	hamming_code::Matrix::Value *decoded_data_elements = decoded_data.GetElements();
	for (size_t i = 0; i < pattern_size; ++i) {
		EXPECT_EQ(data_elements[i], decoded_data_elements[i]);
	}
}

