/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>

typedef struct {
  char* digits;
  size_t num_digits, capacity;
  size_t num_underscores;
} BigInt;

BigInt* bigint_new(void);

void bigint_free(BigInt* bi);

void bigint_add_digit(BigInt* bi, char digit);
void bigint_print(const BigInt* bi);

#endif /* BIGINT_H */
