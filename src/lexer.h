/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef LEXER_H
#define LEXER_H

#include "token.h"

#include <stddef.h>

typedef struct {
  const char* filepath; /* NULL if source not read from file. */
  char* source;
  size_t line_num, col_num;
  size_t pos;

  Token** tokens;
  size_t num_tokens, capacity;
} Lexer;

Lexer* lexer_file(const char* filepath);
Lexer* lexer_source(char* source);

void lexer_free(Lexer* lexer);

#endif /* LEXER_H */
