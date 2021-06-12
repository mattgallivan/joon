/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "bigfloat.h"

#include <stdio.h>
#include <stdlib.h>

BigFloat* bigfloat_new(BigInt* significand, BigInt* exponent) {
  if (!significand || !exponent) return NULL;
  BigFloat* bf = malloc(sizeof(*bf));
  if (!bf) return NULL;
  bf->significand = significand;
  bf->exponent = exponent;
  return bf;
}

void bigfloat_free(BigFloat* bf) {
  if (!bf) return;
  bigint_free(bf->significand);
  bigint_free(bf->exponent);
}

void bigfloat_print(const BigFloat* bf) {
  if (!bf) return;
  bigint_print(bf->significand);
  printf(".");
  bigint_print(bf->exponent);
}
