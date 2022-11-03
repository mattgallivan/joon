#include "lexer.h"

#include <ctype.h>
#include <stdio.h>

bool jn_is_valid_id_body(char c) { return isalpha(c) || isdigit(c) || ((unsigned char)c >= 0x80); }

bool jn_is_valid_id_start(char c) {
  // NOTE: https://www.cprogramming.com/tutorial/unicode.html
  return isalpha(c) || ((unsigned char)c >= 0xC0);
}

int jn_lex(const char *source, int length, int max_out_tokens, JN_Token *out_tokens) {
  int num_tokens = 0;
  int line = 0;
  int column = -1;
  for (int i = 0; i < length; ++i) {
    column++;
    JN_Token *token = &out_tokens[num_tokens];
    char c = source[i];
    switch (c) {
    case ' ': {
      break;
    }
    case '\n': {
      line++;
      column = 0;
      break;
    }
    default:
      if (jn_is_valid_id_start(c)) {
        jn_lex_id(&source[i], length - i, token);
        token->line = line;
        token->column = column;
        i = i + token->id_length;
        column = column + token->id_length;
        num_tokens++;
      }
      break;
    }
  }
  return num_tokens;
}

void jn_lex_id(const char *source, int length, JN_Token *out_token) {
  out_token->id = source;
  out_token->type = JN_IDENTIFIER;
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
  out_token->id_length = id_length;
}

void jn_print_token(JN_Token token) {
  if (token.type == JN_IDENTIFIER) {
    for (int i = 0; i < token.id_length; ++i) {
      printf("%c", token.id[i]);
    }
  } else {
    printf("%s", jn_token_type_names[token.type]);
  }
  printf("(%d, %d)\n", token.line, token.column);
}
