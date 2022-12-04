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
  i64 max_tokens = (i64)(length + 1); // Add one for the END token
  JN_Lexer lexer = {
    .source = source,
    .length = (i64)length,
    .tokens = malloc(sizeof(JN_Token) * max_tokens),
    .num_tokens = 0,
    .capacity = max_tokens,
  };
  if (!lexer.tokens)
    goto handle_lexer_error;

  jn_lex(&lexer);
  jn_print_lexer(lexer, true);

  // Parse the tokens.
  i64 max_nodes = lexer.length;
  JN_Parser parser = {
    .lexer = &lexer,
    .token = lexer.tokens,
    .nodes = malloc(sizeof(JN_Node) * max_nodes),
    .num_nodes = 0,
    .capacity = max_nodes,
  };
  if (!parser.nodes)
    goto handle_parser_error;

  jn_parse(&parser);
  jn_print_parser(parser);

  /*
  int num_nodes = jn_parse(lexer.num_tokens, lexer.tokens, lexer.num_tokens, nodes);
  printf("Num Nodes: %d\n", num_nodes);
  jn_print_tree(nodes[0], 0);

  free(nodes);
  */
  free(lexer.tokens);
  free(source);
  return 0;

handle_parser_error:
  free(lexer.tokens);

handle_lexer_error:
  free(source);

handle_file_error:
  fclose(file);
  return 1;
}
