/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef BIGFLOAT_H
#define BIGFLOAT_H

#include "bigint.h"

typedef struct {
  BigInt* significand;
  BigInt* exponent;
} BigFloat;

BigFloat* bigfloat_new(BigInt* significand, BigInt* exponent);

void bigfloat_free(BigFloat* bf);

void bigfloat_print(const BigFloat* bf);

#endif /* BIGFLOAT_H */
