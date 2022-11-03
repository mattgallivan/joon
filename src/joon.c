#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

int main(int argc, char *argv[]) {
  FILE *file = fopen("../examples/basic.jn", "r");

  // Get the length of the source.
  if (!file || fseek(file, 0, SEEK_END) != 0)
    goto handle_file_error;

  long length = ftell(file);
  if (length == -1 || fseek(file, 0, SEEK_SET) != 0)
    goto handle_file_error;

  // Create a string and copy the source into it.
  char *source = malloc(length);
  if (!source)
    goto handle_file_error;
  if (fread(source, sizeof(char), length, file) != length) {
    free(source);
    goto handle_file_error;
  }
  fclose(file);

  // Lex the source code.
  int max_num_tokens = (int)length;
  JN_Token *tokens = malloc(sizeof(JN_Token) * max_num_tokens);
  int num_tokens = jn_lex(source, (int)length, max_num_tokens, tokens);
  printf("Num Tokens: %d\n", num_tokens);
  for (int i = 0; i < num_tokens; ++i) {
    jn_print_token(tokens[i]);
  }

  free(tokens);
  free(source);
  return 0;

handle_file_error:
  fclose(file);
  return 1;
}
