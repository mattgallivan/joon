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
  JN_TokenType type; // The type of the token.
  int line, column;  // The line and column on which the token begins.

  const char *characters; // The beginning of characters stored in the token. Not null terminated!
  int length;             // The number of characters stored in the token.
} JN_Token;

/**
 * @brief Check if the character is valid inside an identifier.
 *
 * @param c The character to check (not the first character).
 * @return True if the character can be used inside an identifier.
 */
bool jn_is_valid_id_body(char c);

/**
 * @brief Check if the character is a valid start to an identifier.
 *
 * @param c The lead character to check.
 * @return True if the character can be used at the start of an identifier.
 */
bool jn_is_valid_id_start(char c);

/**
 * @brief Check if the character is a valid start to an integer.
 *
 * @param c The lead character to check.
 * @return True if the character can be used at the start of an integer.
 */
bool jn_is_valid_number_start(char c);

/**
 * @brief Convert source code into an array of tokens.
 *
 * @param length The length of the source code.
 * @param source The source code to lex.
 * @param max_out_tokens The maximum number of tokens provided by out_tokens'
 * memory.
 * @param out_tokens Memory allocated by the caller to store the lexed tokens.
 * @return The number of tokens lexed.
 */
int jn_lex(int length, const char *source, int max_out_tokens, JN_Token *out_tokens);

/**
 * @brief Lex an identifier.
 *
 * @param source The start of the source code to lex.
 * @param length The length of the source code.
 * @param out_token Memory allocated by the caller to store the lexed token.
 */
void jn_lex_id(const char *source, int length, JN_Token *out_token);

/**
 * @brief Lex a number (float or integer).
 *
 * @param source The start of the source code to lex.
 * @param length The length of the source code.
 * @param out_token Memory allocated by the caller to store the lexed token.
 */
void jn_lex_number(const char *source, int length, JN_Token *out_token);

/**
 * @brief Print a token.
 *
 * @param token The token to print.
 */
void jn_print_token(JN_Token token);

#endif /* LEXER_H */
