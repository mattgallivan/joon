#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

typedef enum {
  JN_TT_UNKNOWN,
  JN_TT_END,
  JN_TT_ERROR,
  JN_TT_EQUAL,
  JN_TT_IDENTIFIER,
  JN_TT_INTEGER,
  JN_TT_SEMICOLON,
} JN_TokenType;

static const char *jn_token_type_names[7] = {
    [JN_TT_UNKNOWN] = "UNKNOWN", [JN_TT_END] = "END",     [JN_TT_EQUAL] = "=",   [JN_TT_IDENTIFIER] = "ID",
    [JN_TT_INTEGER] = "INT",     [JN_TT_SEMICOLON] = ";", [JN_TT_ERROR] = "ERR",
};

typedef struct {
  JN_TokenType type;      // The type of the token.
  int line, column;       // The line and column on which the token begins.
  const char *characters; // The beginning of characters stored in the token. Not null terminated!
  int length;             // The number of characters stored in the token.
} JN_Token;

typedef struct {
  const char *source; // The source code.
  int length;         // The length of the source code.
  JN_Token *tokens;   // The lexed tokens.
  int num_tokens;     // The number of lexed tokens.
} JN_Lexer;

bool jn_is_valid_id_body(char c);

bool jn_is_valid_id_start(char c);

bool jn_is_valid_number_start(char c);

void jn_lex(JN_Lexer *lexer);

void jn_lex_id(const char *source, int length, JN_Token *out_token);

void jn_lex_number(const char *source, int length, JN_Token *out_token);

void jn_print_lexer(JN_Lexer lexer, bool print_tokens);

void jn_print_token(JN_Token token);

#endif /* LEXER_H */
