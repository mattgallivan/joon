#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "parser.h"

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
  JN_Lexer lexer = {
    .source = source,
    .length = length + 1, // Add one for the END token
    .current = 0,
    .tokens = malloc(sizeof(JN_Token) * (int)(length + 1)),
    .num_tokens = 0,
  };
  if (!lexer.tokens)
    goto handle_lexer_error;

  jn_lex(&lexer);
  jn_print_lexer(lexer, true);

  // Parse the tokens.
  /*
  JN_Node *nodes = malloc(sizeof(*nodes) * lexer.num_tokens);
  if (!nodes)
    goto handle_parser_error;

  int num_nodes = jn_parse(lexer.num_tokens, lexer.tokens, lexer.num_tokens, nodes);
  printf("Num Nodes: %d\n", num_nodes);
  jn_print_tree(nodes[0], 0);

  free(nodes);
  */
  free(lexer.tokens);
  free(source);
  return 0;

  /*
handle_parser_error:
  free(lexer.tokens);
  */

handle_lexer_error:
  free(source);

handle_file_error:
  fclose(file);
  return 1;
}
