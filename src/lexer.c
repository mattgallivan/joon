/* Copyright (c) 2021 Matthew Gallivan <gallivan.matt@gmail.com>
 * You may use, distribute and modify this code under the
 * terms of the MIT license.
 */

#include "lexer.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline char next(Lexer* lexer) {
  if (!lexer) return '\0';
  return lexer->source[lexer->pos++];
}

static inline char peek(const Lexer* lexer) {
  if (!lexer) return '\0';
  return lexer->source[lexer->pos];
}

static inline void previous(Lexer* lexer) {
  if (!lexer) return;
  --lexer->pos;
}

static void add(Lexer* lexer, Token* token) {
  if (!lexer || !token) return;
  token->line_num = lexer->line_num;
  token->col_num = lexer->col_num;
  lexer->tokens[lexer->num_tokens++] = token;
  if (lexer->num_tokens == lexer->capacity) {
    lexer->capacity *= 2;
    // TODO: Error check on realloc.
    lexer->tokens =
        realloc(lexer->tokens, sizeof(*lexer->tokens) * lexer->capacity);
  }
}

static inline TokenType get_id_type(char* string) {
  if (strncmp("and", string, 3) == 0) return TOKEN_TYPE_KWORD_AND;
  if (strncmp("break", string, 5) == 0) return TOKEN_TYPE_KWORD_BREAK;
  if (strncmp("continue", string, 8) == 0) return TOKEN_TYPE_KWORD_CONTINUE;
  if (strncmp("else", string, 4) == 0) return TOKEN_TYPE_KWORD_ELSE;
  if (strncmp("enum", string, 4) == 0) return TOKEN_TYPE_KWORD_ENUM;
  if (strncmp("false", string, 5) == 0) return TOKEN_TYPE_KWORD_FALSE;
  if (strncmp("fun", string, 3) == 0) return TOKEN_TYPE_KWORD_FUN;
  if (strncmp("for", string, 3) == 0) return TOKEN_TYPE_KWORD_FOR;
  if (strncmp("if", string, 2) == 0) return TOKEN_TYPE_KWORD_IF;
  if (strncmp("let", string, 3) == 0) return TOKEN_TYPE_KWORD_LET;
  if (strncmp("match", string, 5) == 0) return TOKEN_TYPE_KWORD_MATCH;
  if (strncmp("mut", string, 3) == 0) return TOKEN_TYPE_KWORD_MUT;
  if (strncmp("none", string, 4) == 0) return TOKEN_TYPE_KWORD_NONE;
  if (strncmp("or", string, 2) == 0) return TOKEN_TYPE_KWORD_OR;
  if (strncmp("rec", string, 3) == 0) return TOKEN_TYPE_KWORD_REC;
  if (strncmp("ret", string, 3) == 0) return TOKEN_TYPE_KWORD_RET;
  if (strncmp("true", string, 4) == 0) return TOKEN_TYPE_KWORD_TRUE;
  if (strncmp("while", string, 5) == 0) return TOKEN_TYPE_KWORD_WHILE;
  return TOKEN_TYPE_ID;
}

static inline char* identifier(Lexer* lexer, char c) {
  if (!lexer) return NULL;
  size_t capacity = 10;
  char* identifier = malloc(sizeof(*identifier) * capacity);
  if (!identifier) return NULL;
  size_t length = 0;
  do {
    identifier[length++] = c;
    c = next(lexer);
    if (length == capacity) {
      capacity *= 2;
      // TODO: Error check on realloc.
      identifier = realloc(identifier, sizeof(*identifier) * capacity);
    }
  } while (isalpha(c) || isdigit(c) || c == '_');
  identifier[length] = '\0';
  previous(lexer);
  return identifier;
}

static inline char* string(Lexer* lexer, char c) {
  if (!lexer) return NULL;
  size_t capacity = 10;
  char* string = malloc(sizeof(*string) * capacity);
  if (!string) return NULL;
  size_t length = 0;
  c = next(lexer);
  do {
    string[length++] = c;
    c = next(lexer);
    if (length == capacity) {
      capacity *= 2;
      // TODO: Error check on realloc.
      string = realloc(string, sizeof(*string) * capacity);
    }
  } while (c != '"');
  string[length] = '\0';
  return string;
}

Lexer* lexer_file(const char* filepath) {
  if (!filepath) return NULL;
  FILE* file = fopen(filepath, "r");
  if (!file) return NULL;

  // Determine length of source.
  // NOTE: This won't work on large files because of fseek.
  if (fseek(file, 0, SEEK_END) == -1) return NULL;
  long length = ftell(file);
  if (fseek(file, 0, SEEK_SET) == -1) return NULL;

  // Read the source contents into a string.
  char* source = malloc(sizeof(*source) * length + 1);
  if (!source) return NULL;
  size_t amount = fread(source, sizeof(*source), length, file);
  if (amount != sizeof(*source) * length) return NULL;
  source[length] = '\0';

  fclose(file);

  Lexer* lexer = lexer_source(source);
  return lexer;
}

Lexer* lexer_source(const char* source) {
  if (!source) return NULL;

  // Initialize the lexer.
  Lexer* lexer = malloc(sizeof(*lexer));
  if (!lexer) return NULL;
  lexer->filepath = NULL;
  lexer->source = source;
  lexer->line_num = 0;
  lexer->col_num = 0;
  lexer->pos = 0;

  // Create a dynamic array of tokens.
  lexer->num_tokens = 0;
  lexer->capacity = 1024;
  lexer->tokens = malloc(sizeof(*lexer->tokens) * lexer->capacity);
  if (!lexer->tokens) {
    free(lexer);
    return NULL;
  }

  // Iterate through the source and create the tokens.
  char c;
  while ((c = next(lexer)) != '\0') {
    switch (c) {
    case '\n':
      lexer->col_num = 0;
      lexer->line_num++;
      // NOTE: We don't want to increment the column number below.
      continue;
    case '\t':
      break;
    case ' ':
      break;
    case '&':
      add(lexer, token_atom(TOKEN_TYPE_AMPERSAND));
      break;
    case '@':
      add(lexer, token_atom(TOKEN_TYPE_AT));
      break;
    case '!':
      add(lexer, token_atom(TOKEN_TYPE_BANG));
      break;
    case '{':
      add(lexer, token_atom(TOKEN_TYPE_BRACE_L));
      break;
    case '}':
      add(lexer, token_atom(TOKEN_TYPE_BRACE_R));
      break;
    case '[':
      add(lexer, token_atom(TOKEN_TYPE_BRACKET_L));
      break;
    case ']':
      add(lexer, token_atom(TOKEN_TYPE_BRACKET_R));
      break;
    case '^':
      add(lexer, token_atom(TOKEN_TYPE_CARET));
      break;
    case ':':
      add(lexer, token_atom(TOKEN_TYPE_COLON));
      break;
    case ',':
      add(lexer, token_atom(TOKEN_TYPE_COMMA));
      break;
    case '-':
      if (peek(lexer) == '>') {
        add(lexer, token_atom(TOKEN_TYPE_ARROW));
        lexer->col_num++;
	next(lexer);
      } else {
        add(lexer, token_atom(TOKEN_TYPE_DASH));
      }
      break;
    case '.':
      add(lexer, token_atom(TOKEN_TYPE_DOT));
      break;
    case '=':
      add(lexer, token_atom(TOKEN_TYPE_EQUAL));
      break;
    case '>':
      add(lexer, token_atom(TOKEN_TYPE_GT));
      break;
    case '#':
      add(lexer, token_atom(TOKEN_TYPE_HASH));
      break;
    case '<':
      add(lexer, token_atom(TOKEN_TYPE_LT));
      break;
    case '(':
      add(lexer, token_atom(TOKEN_TYPE_PAREN_L));
      break;
    case ')':
      add(lexer, token_atom(TOKEN_TYPE_PAREN_R));
      break;
    case '%':
      add(lexer, token_atom(TOKEN_TYPE_PERCENT));
      break;
    case '|':
      add(lexer, token_atom(TOKEN_TYPE_PIPE));
      break;
    case '+':
      add(lexer, token_atom(TOKEN_TYPE_PLUS));
      break;
    case '?':
      add(lexer, token_atom(TOKEN_TYPE_QUESTION));
      break;
    case ';':
      add(lexer, token_atom(TOKEN_TYPE_SEMICOLON));
      break;
    case '/':
      add(lexer, token_atom(TOKEN_TYPE_SLASH_F));
      break;
    case '*':
      add(lexer, token_atom(TOKEN_TYPE_STAR));
      break;
    case '~':
      add(lexer, token_atom(TOKEN_TYPE_TILDE));
      break;
    case '"': {
      char* str = string(lexer, c);
      add(lexer, token_string(TOKEN_TYPE_STRING, str));
      lexer->col_num += strlen(str) + 2 - 1;
      break;
    }
    default:
      if (isalpha(c)) {
        char* id = identifier(lexer, c);
        add(lexer, token_string(get_id_type(id), id));
        lexer->col_num += strlen(id) - 1;
      } else {
        add(lexer, token_atom(TOKEN_TYPE_UNKNOWN));
      }
      break;
    }
    lexer->col_num++;
  }
  add(lexer, token_atom(TOKEN_TYPE_END));
  return lexer;
}

void lexer_free(Lexer* lexer) {
  if (!lexer) return;
  for (size_t i = 0; i < lexer->num_tokens; ++i) {
    token_free(lexer->tokens[i]);
  }
  free(lexer->tokens);
  free(lexer);
}
