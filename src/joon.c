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
  int max_num_tokens = (int)length;
  JN_Token *tokens = malloc(sizeof(*tokens) * max_num_tokens);
  if (!tokens) {
    free(source);
    return 1;
  }
  int num_tokens = jn_lex((int)length, source, max_num_tokens, tokens);
  printf("Num Tokens: %d\n", num_tokens);
  for (int i = 0; i < num_tokens; ++i) {
    jn_print_token(tokens[i]);
  }
  printf("\n");

  // Parse the tokens.
  int max_num_nodes = max_num_tokens;
  JN_Node *nodes = malloc(sizeof(*nodes) * max_num_nodes);
  if (!nodes) {
    free(tokens);
    free(source);
    return 1;
  }
  int num_nodes = jn_parse(num_tokens, tokens, max_num_nodes, nodes);
  printf("Num Nodes: %d\n", num_nodes);
  jn_print_tree(nodes[0], 0);

  free(nodes);
  free(tokens);
  free(source);
  return 0;

handle_file_error:
  fclose(file);
  return 1;
}
