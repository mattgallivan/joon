#include "parser.h"

#include <stdio.h>
#include <string.h>

bool jn_is_token(JN_Token token, JN_TokenType type, const char *characters) {
  bool is_type = token.type == type;
  if (characters) {
    return is_type && (strncmp(characters, token.characters, token.length) == 0);
  }
  return is_type;
}

int jn_parse(int num_tokens, JN_Token *tokens, int max_num_nodes, JN_Node *nodes) {
  int num_nodes = 0;
  if (num_tokens == 0 || max_num_nodes == 0 || !tokens || !nodes) {
    return num_nodes;
  }

  num_nodes += jn_parse_assignment(num_tokens, tokens, num_nodes, max_num_nodes, nodes);
  return num_nodes;
}

int jn_parse_assignment(int num_tokens, JN_Token *tokens, int num_nodes, int max_num_nodes, JN_Node *nodes) {
  // Ensure we have enough nodes.
  int num_nodes_to_create = 3;
  if (max_num_nodes - num_nodes < num_nodes_to_create) {
    return -1;
  }
  
  // Ensure we have enough tokens for a valid parse.
  int num_tokens_to_consume = 4;
  for (int i = 0; i < num_tokens_to_consume; ++i) {
    if (tokens[i].type == JN_TT_END) return -1;
  }

  // If the parse is successful, create the nodes.
  if (jn_is_token(tokens[0], JN_TT_IDENTIFIER, "let") &&
      jn_is_token(tokens[1], JN_TT_IDENTIFIER, NULL) &&
      jn_is_token(tokens[2], JN_TT_EQUAL, NULL) &&
      jn_is_token(tokens[3], JN_TT_INTEGER, NULL)) {
    
    nodes[0] = (JN_Node){
      .type = JN_NT_ASSIGNMENT,
      .id_node = &nodes[1],
      .value_node = &nodes[2],
    };

    nodes[1] = (JN_Node){
      .type = JN_NT_IDENTIFIER,
    };

    nodes[2] = (JN_Node){
      .type = JN_NT_INTEGER,
    };
    
    return num_tokens_to_consume;
  }

  return 0;
}

void jn_print_tree(JN_Node node, int depth) {
  for (int i = 0; i < depth; ++i) {
    printf("\t");
  }
  printf("%s\n", jn_node_type_names[node.type]);
  if (node.id_node) {
    jn_print_tree(*node.id_node, depth + 1);
  }
  if (node.value_node) {
    jn_print_tree(*node.value_node, depth + 1);
  }
}