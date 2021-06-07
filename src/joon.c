/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "lexer.h"

#include <stdio.h>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Usage: ./joon file.jn\n");
    return 1;
  }

  Lexer* lexer = lexer_file(argv[1]);
  if (!lexer) return 1;

  for (size_t i = 0; i < lexer->num_tokens; ++i) {
    token_print(lexer->tokens[i]);
  }
  lexer_free(lexer);
  return 0;
}
