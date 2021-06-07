/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "token.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* String representations of each token. */
static char* token_type_strings[] = {
#define t(tok, str) [TOKEN_TYPE_##tok] = str,
    TOKEN_LIST
#undef t
};

static bool is_string_type(TokenType type) {
  return type == TOKEN_TYPE_ID || type == TOKEN_TYPE_STRING;
}

static Token* token_new(TokenType type) {
  Token* token = malloc(sizeof(*token));
  if (!token) return NULL;
  token->type = type;
  token->line_num = 0;
  token->col_num = 0;
  return token;
}

Token* token_atom(TokenType type) { return token_new(type); }

Token* token_string(TokenType type, char* s, size_t length) {
  if (!s) return NULL;
  Token* token = token_new(type);
  if (!token) return NULL;
  token->data.as_string.s = s;
  token->data.as_string.length = length;
  return token;
}

void token_free(Token* token) {
  if (!token) return;
  if (is_string_type(token->type)) { free(token->data.as_string.s); }
  // TODO: Free integer data
  // TODO: Free float data
  free(token);
}

void token_print(const Token* token) {
  if (!token) return;
  if (is_string_type(token->type)) {
    printf("(%s(%zu): %s, %zu, %zu)\n", token_type_strings[token->type],
           token->data.as_string.length, token->data.as_string.s,
           token->line_num, token->col_num);
  } else {
    printf("(%s, %zu, %zu)\n", token_type_strings[token->type],
           token->line_num, token->col_num);
  }
}
