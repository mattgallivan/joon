#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#include <stdint.h>

typedef enum {
  JN_NT_UNKNOWN,
  JN_NT_ERROR,
  JN_NT_ASSIGNMENT,
  JN_NT_IDENTIFIER,
  JN_NT_INTEGER,
} JN_NodeType;

static const char *jn_node_type_names[5] = {
    [JN_NT_UNKNOWN] = "UNKNOWN",       [JN_NT_ERROR] = "ERR",       [JN_NT_ASSIGNMENT] = "ASSIGNMENT",
    [JN_NT_IDENTIFIER] = "IDENTIFIER", [JN_NT_INTEGER] = "INTEGER",
};

typedef struct JN_Node JN_Node;
struct JN_Node {
  JN_NodeType type;
  JN_Token *token;

  JN_Node *id_node;
  JN_Node *value_node;
  int64_t integer;
};

bool jn_is_token(JN_Token token, JN_TokenType type, const char *characters);

int jn_parse(int num_tokens, JN_Token *tokens, int max_num_nodes, JN_Node *nodes);

int jn_parse_assignment(int num_tokens, JN_Token *tokens, int num_nodes, int max_num_nodes, JN_Node *nodes);

void jn_print_node(JN_Node node);

void jn_print_tree(JN_Node node, int depth);

int64_t jn_string_to_int64(int length, const char *string);

#endif /* PARSER_H */
