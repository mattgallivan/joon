#include "lexer.h"

#include <ctype.h>
#include <stdio.h>

bool jn_is_valid_id_body(char c) { return isalpha(c) || isdigit(c) || ((unsigned char)c >= 0x80); }

bool jn_is_valid_id_start(char c) {
  // NOTE: https://www.cprogramming.com/tutorial/unicode.html
  return isalpha(c) || ((unsigned char)c >= 0xC0);
}

bool jn_is_valid_number_start(char c) { return c == '-' || isdigit(c); }

int jn_lex(int length, const char *source, int max_out_tokens, JN_Token *out_tokens) {
  int num_tokens = 0;
  int line = 1;
  int column = 1;

  // Iterate until length - 1 to save room for END token.
  for (int i = 0; i < length - 1; ++i) {
    JN_Token *token = &out_tokens[num_tokens];
    token->type = JN_TT_UNKNOWN;

    char c = source[i];
    switch (c) {
    case '=': {
      token->type = JN_TT_EQUAL;
      break;
    }
    case ';': {
      token->type = JN_TT_SEMICOLON;
      break;
    }
    case ' ': {
      break;
    }
    case '\n': {
      line++;
      column = 1;
      break;
    }
    default:
      if (jn_is_valid_id_start(c)) {
        jn_lex_id(&source[i], length - i, token);
      } else if (jn_is_valid_number_start(c)) {
        jn_lex_number(&source[i], length - i, token);
      }
      break;
    }

    if (token->type != JN_TT_UNKNOWN) {
      num_tokens++;
    }

    token->line = line;
    token->column = column;
    if (token->type == JN_TT_IDENTIFIER || token->type == JN_TT_INTEGER) {
      i = i + token->length;
      column = column + token->length;
    }
    column++;
  }

  out_tokens[num_tokens++] = (JN_Token){
      .type = JN_TT_END,
      .line = line,
      .column = column,
  };

  return num_tokens;
}

void jn_lex_id(const char *source, int length, JN_Token *out_token) {
  out_token->characters = source;
  out_token->type = JN_TT_IDENTIFIER;
  int id_length = 1;
  source++;
  while (id_length < length && source) {
    if (jn_is_valid_id_body(*source)) {
      id_length++;
      source++;
    } else {
      break;
    }
  }
  out_token->length = id_length;
}

void jn_lex_number(const char *source, int length, JN_Token *out_token) {
  out_token->characters = source;
  out_token->type = JN_TT_INTEGER;
  int integer_length = 1;
  source++;
  while (integer_length < length && source) {
    if (isdigit(*source)) {
      integer_length++;
      source++;
    } else {
      break;
    }
  }
  out_token->length = integer_length;

  // If we've only parsed a negative sign, this isn't a number.
  if (out_token->characters[0] == '-' && integer_length == 1) {
    out_token->type = JN_TT_ERROR;
  }
}

void jn_print_token(JN_Token token) {
  if (token.type == JN_TT_IDENTIFIER || token.type == JN_TT_INTEGER || token.type == JN_TT_ERROR) {
    printf("%s(", jn_token_type_names[token.type]);
    for (int i = 0; i < token.length; ++i) {
      printf("%c", token.characters[i]);
    }
    printf(")");
  } else {
    printf("%s", jn_token_type_names[token.type]);
  }
  printf("(%d, %d)\n", token.line, token.column);
}
