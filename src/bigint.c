/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "bigint.h"

#include <stdlib.h>

BigInt* bigint_new(void) {
  BigInt* bi = malloc(sizeof(*bi));
  if (!bi) return NULL;
  bi->num_digits = 0;
  bi->capacity = 16;
  bi->num_underscores = 0;
  bi->digits = malloc(sizeof(*bi->digits) * bi->capacity);
  if (!bi->digits) {
    bigint_free(bi);
    return NULL;
  }
  return bi;
}

void bigint_free(BigInt* bi) {
  if (!bi) return;
  free(bi->digits);
  free(bi);
}

void bigint_add_digit(BigInt* bi, char digit) {
  if (!bi) return;
  bi->digits[bi->num_digits++] = digit;
  if (bi->num_digits == bi->capacity) {
    bi->capacity *= 2;
    char* tmp = realloc(bi->digits, sizeof(*tmp) * bi->capacity);
    if (!tmp) {
      bigint_free(bi);
      return;
    }
    bi->digits = tmp;
  }
}
