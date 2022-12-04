#ifndef LEXER_H
#define LEXER_H

#include "types.h"
#include <stdbool.h>

typedef enum {
  JN_TT_UNKNOWN,
  JN_TT_DASH,
  JN_TT_END,
  JN_TT_ERROR,
  JN_TT_EQUAL,
  JN_TT_IDENTIFIER,
  JN_TT_INTEGER,
  JN_TT_SEMICOLON,
} JN_TokenType;

static const char *const jn_token_type_names[] = {
  [JN_TT_UNKNOWN] = "UNK",
  [JN_TT_DASH] = "-",
  [JN_TT_END] = "END",
  [JN_TT_EQUAL] = "=",
  [JN_TT_IDENTIFIER] = "ID",
  [JN_TT_INTEGER] = "INT",
  [JN_TT_SEMICOLON] = ";",
  [JN_TT_ERROR] = "ERR",
};

typedef struct {
  JN_TokenType type;      // The type of the token.
  i64 line, column;       // The line and column on which the token begins.
  const char *characters; // The beginning of characters stored in the token. Not null terminated!
  i64 length;             // The number of characters stored in the token.
} JN_Token;

typedef struct {
  const char *source; // The source code.
  i64 length;         // The length of the source code.
  i64 current;        // The index of the character currently being lexed.
  JN_Token *tokens;   // The lexed tokens. This space is provided by the user.
  i64 num_tokens;     // The number of lexed tokens.
  i64 capacity;       // The maximum number of tokens that can be stored.
} JN_Lexer;

/**
 * @brief Lex source code i64o tokens.
 *
 * @param lexer The lexer containing the source code and allocated memory for tokens.
 */
void jn_lex(JN_Lexer *lexer);

/**
 * @brief Print a lexer.
 *
 * @param lexer The lexer to print.
 * @param print_tokens If true, print all tokens contained in the lexer.
 */
void jn_print_lexer(JN_Lexer lexer, bool print_tokens);

/**
 * @brief Print a token.
 *
 * @param token The token to print.
 */
void jn_print_token(JN_Token token);

#endif /* LEXER_H */
