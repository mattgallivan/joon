/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "bigfloat.h"
#include "bigint.h"

#include <stddef.h>
#include <stdint.h>

/* All possible token types and their string representations. */

#define TOKEN_LIST  \
  t(AMPERSAND, "&") \
  t(ARROW, "->") \
  t(AT, "@") \
  t(BANG, "!") \
  t(BRACE_L, "{") \
  t(BRACE_R, "}") \
  t(BRACKET_L, "[") \
  t(BRACKET_R, "]") \
  t(CARET, "^") \
  t(COLON, ":") \
  t(COMMA, ",") \
  t(DASH, "-") \
  t(DOT, ".") \
  t(EQUAL, "=") \
  t(GT, ">") \
  t(HASH, "#") \
  t(ID, "ID") \
  t(INTEGER, "INTEGER") \
  t(LT, "<") \
  t(PAREN_L, "(") \
  t(PAREN_R, ")") \
  t(PERCENT, "%") \
  t(PIPE, "|") \
  t(PLUS, "+") \
  t(QUESTION, "?") \
  t(SEMICOLON, ";") \
  t(SLASH_F, "/") \
  t(STAR, "*") \
  t(STRING, "STRING") \
  t(TILDE, "~") \
  t(KWORD_AND, "and") \
  t(KWORD_BREAK, "break") \
  t(KWORD_CONTINUE, "continue") \
  t(KWORD_ELSE, "else") \
  t(KWORD_ENUM, "enum") \
  t(KWORD_FALSE, "false") \
  t(KWORD_FUN, "fun") \
  t(KWORD_FOR, "for") \
  t(KWORD_IF, "if") \
  t(KWORD_LET, "let") \
  t(KWORD_MATCH, "match") \
  t(KWORD_MUT, "mut") \
  t(KWORD_NONE, "none") \
  t(KWORD_OR, "or") \
  t(KWORD_REC, "rec") \
  t(KWORD_RET, "ret") \
  t(KWORD_TRUE, "true") \
  t(KWORD_WHILE, "while") \
  t(END, "end") \
  t(UNKNOWN, "UNKNOWN")

typedef enum {
#define t(tok, str) TOKEN_TYPE_##tok,
  TOKEN_LIST
#undef t
} TokenType;

/* Data for tokens that must also store literal values alongside their type. */

typedef struct {
  uint32_t c;
} TokenDataChar;

typedef struct {
  BigFloat* f;
} TokenDataFloat;

typedef struct {
  BigInt* i;
} TokenDataInt;

typedef struct {
  char* s;
  size_t length;
} TokenDataString;

/* Tokens are the fundamental unit of the lexing stage. */

typedef struct {
  TokenType type;
  union {
    TokenDataChar as_char;
    TokenDataFloat as_float;
    TokenDataInt as_int;
    TokenDataString as_string;
  } data;

  size_t line_num, col_num;
} Token;

Token* token_atom(TokenType type);
Token* token_integer(TokenType type, BigInt* bi);
Token* token_string(TokenType type, char* s, size_t length);

void token_free(Token* token);
void token_print(const Token* token);

#endif /* TOKEN_H */
