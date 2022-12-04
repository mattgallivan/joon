#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

static JN_Token *next(JN_Parser *parser) {
  if (!parser) {
    printf("parser::next() - No parser found!\n");
    return NULL;
  }

  if (parser->token->type == JN_TT_END) {
    printf("parser::next() - Exceeded lexer capacity!\n");
    return NULL;
  }

  return parser->token++;
}

static bool accept(JN_Parser *parser, JN_TokenType type) {
  if (!parser) return false;

  JN_Token *token = next(parser);
  if (!token) return false;
  return token->type == type;
}

static bool accept_id(JN_Parser *parser, const char *str) {
  if (!parser || !str) return false;

  JN_Token *token = next(parser);
  if (token->type != JN_TT_IDENTIFIER) return false;

  return strncmp(str, token->characters, token->length) == 0;
}

void jn_parse(JN_Parser *parser) {
  if (!parser) return;

  // LET
  if (!accept_id(parser, "let")) return;

  // ID
  JN_Token *id = next(parser);
  if (!id || id->type != JN_TT_IDENTIFIER) return;
  // TODO(matt): Use token value.

  // =
  if (!accept(parser, JN_TT_EQUAL)) return;

  // INTEGER
  JN_Token *integer = next(parser);
  if (!integer || integer->type != JN_TT_INTEGER) return;
  // TODO(matt): Use token value.

  // SEMICOLON
  if (!accept(parser, JN_TT_SEMICOLON)) return;

  /* TODO(matt): Come up with a good way of storing nodes linearly.
  parser->nodes[parser->num_nodes++] = (JN_Node){
    .type = JN_NT_ASSIGNMENT,
    .id_node = id_node,
    .value_node = value_node,
  };
  */
}

void jn_print_parser(JN_Parser parser) {
  printf("+------------+\n");
  printf("| Parser     |\n");
  printf("+------------+\n");
  printf("  num_nodes: %ld\n", parser.num_nodes);
}

/*
bool jn_is_token(JN_Token token, JN_TokenType type, const char *characters) {
  bool is_type = token.type == type;
  if (characters) {
    return is_type && (strncmp(characters, token.characters, token.length) == 0);
  }
  return is_type;
}

i64 jn_parse(i64 num_tokens, JN_Token *tokens, i64 max_num_nodes, JN_Node *nodes) {
  i64 num_nodes = 0;
  if (num_tokens == 0 || max_num_nodes == 0 || !tokens || !nodes) {
    return num_nodes;
  }

  num_nodes += jn_parse_assignment(num_tokens, tokens, num_nodes, max_num_nodes, nodes);
  return num_nodes;
}

i64 jn_parse_assignment(i64 num_tokens, JN_Token *tokens, i64 num_nodes, i64 max_num_nodes, JN_Node *nodes) {
  // Ensure we have enough nodes.
  i64 num_nodes_to_create = 3;
  if (max_num_nodes - num_nodes < num_nodes_to_create) {
    return -1;
  }

  // Ensure we have enough tokens for a valid parse.
  i64 num_tokens_to_consume = 4;
  for (i64 i = 0; i < num_tokens_to_consume; ++i) {
    if (tokens[i].type == JN_TT_END)
      return -1;
  }

  // If the parse is successful, create the nodes.
  if (jn_is_token(tokens[0], JN_TT_IDENTIFIER, "let") && jn_is_token(tokens[1], JN_TT_IDENTIFIER, NULL) &&
      jn_is_token(tokens[2], JN_TT_EQUAL, NULL) && jn_is_token(tokens[3], JN_TT_INTEGER, NULL)) {

    nodes[0] = (JN_Node){
        .type = JN_NT_ASSIGNMENT,
        .id_node = &nodes[1],
        .value_node = &nodes[2],
        .token = NULL,
    };

    nodes[1] = (JN_Node){
        .type = JN_NT_IDENTIFIER,
        .token = &tokens[1],
    };

    nodes[2] = (JN_Node){
        .type = JN_NT_INTEGER,
        .token = &tokens[3],
        .integer = jn_string_to_i64(tokens[3].length, tokens[3].characters),
    };

    return num_tokens_to_consume;
  }

  return 0;
}

void jn_print_node(JN_Node node) {
  printf("%s", jn_node_type_names[node.type]);
  switch (node.type) {
  case JN_NT_IDENTIFIER: {
    printf("(");
    for (i64 i = 0; i < node.token->length; ++i) {
      printf("%c", node.token->characters[i]);
    }
    printf(")");
    break;
  }
  case JN_NT_INTEGER: {
    printf("(%ld)", node.integer);
    break;
  }
  default:
    break;
  }
  printf("\n");
}

void jn_print_tree(JN_Node node, i64 depth) {
  for (i64 i = 0; i < depth; ++i) {
    printf("\t");
  }

  jn_print_node(node);

  if (node.id_node) {
    jn_print_tree(*node.id_node, depth + 1);
  }
  if (node.value_node) {
    jn_print_tree(*node.value_node, depth + 1);
  }
}

i64 jn_string_to_i64(i64 length, const char *string) {
  if (length <= 0 || !string)
    return -1;

  bool is_negative = string[0] == '-';

  i64 integer = 0;
  for (i64 i = 1; i < length; ++i) {
    integer = integer * 10 + (string[i] - '0');
  }

  return is_negative ? -1 * integer : integer;
}
*/
