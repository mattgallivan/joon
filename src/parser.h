#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#include "types.h"

typedef enum {
  JN_NT_UNKNOWN,
  JN_NT_ERROR,
  JN_NT_ASSIGNMENT,
  JN_NT_IDENTIFIER,
  JN_NT_INTEGER,
} JN_NodeType;

static const char *const jn_node_type_names[5] = {
  [JN_NT_UNKNOWN] = "UNKNOWN",
  [JN_NT_ERROR] = "ERR",
  [JN_NT_ASSIGNMENT] = "ASSIGNMENT",
  [JN_NT_IDENTIFIER] = "IDENTIFIER",
  [JN_NT_INTEGER] = "INTEGER",
};

typedef struct JN_Node JN_Node;
struct JN_Node {
  JN_NodeType type;
  JN_Token *token;

  JN_Node *id_node;
  JN_Node *value_node;
  i64 integer;
};

typedef struct {
  const JN_Lexer *lexer;
  JN_Token *token;
  JN_Node *nodes;
  i64 num_nodes;
  i64 capacity;
} JN_Parser;

void jn_parse(JN_Parser *parser);

void jn_print_parser(JN_Parser parser);

/*
bool jn_is_token(JN_Token token, JN_TokenType type, const char *characters);

i64 jn_parse(i64 num_tokens, JN_Token *tokens, i64 max_num_nodes, JN_Node *nodes);

i64 jn_parse_assignment(i64 num_tokens, JN_Token *tokens, i64 num_nodes, i64 max_num_nodes, JN_Node *nodes);

void jn_print_node(JN_Node node);

void jn_print_tree(JN_Node node, i64 depth);

i64 jn_string_to_i64(i64 length, const char *string);
*/

#endif /* PARSER_H */
