/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef BIGINT_H
#define BIGINT_H

#include <stddef.h>

typedef struct {
  unsigned char* digits;
  size_t num_digits, capacity;
} BigInt;

#endif /* BIGINT_H */
