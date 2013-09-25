/*
 * MatrixTest.cpp
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
#include "../src/Matrix.h"

using namespace std;

class MatrixTest: public ::testing::Test {
protected:
	virtual void SetUp() {
		size_t number_rows = 2;
		size_t number_columns = 2;
		matrix0 = hamming_code::Matrix(number_rows, number_columns);
		matrix0.SetElement(0, 0, 0);
		matrix0.SetElement(0, 1, 1);
		matrix0.SetElement(1, 0, 2);
		matrix0.SetElement(1, 1, 3);
		matrix1 = hamming_code::Matrix(number_rows, number_columns);
		matrix1.SetElement(0, 0, 1);
		matrix1.SetElement(0, 1, 0);
		matrix1.SetElement(1, 0, 0);
		matrix1.SetElement(1, 1, 1);
	}

	virtual void TearDown() {
	}
	hamming_code::Matrix matrix0;
	hamming_code::Matrix matrix1;
};

TEST_F(MatrixTest, Multiply) {
	hamming_code::Matrix ret;
	hamming_code::Matrix::Multiply(matrix0, matrix1, &ret);
	EXPECT_EQ(0, ret.GetElement(0, 0));
	EXPECT_EQ(1, ret.GetElement(0, 1));
	EXPECT_EQ(2, ret.GetElement(1, 0));
	EXPECT_EQ(3, ret.GetElement(1, 1));
}

