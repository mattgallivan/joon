/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "lexer.h"

int main(void) {
  Lexer* lexer = lexer_file("../examples/basic.jn");
  if (!lexer) return 1;

  for (size_t i = 0; i < lexer->num_tokens; ++i) {
    token_print(lexer->tokens[i]);
  }
  lexer_free(lexer);
  return 0;
}
