#include "lexer.h"

#include <ctype.h>

bool jn_is_valid_id_body(char c) { return isalpha(c) || isdigit(c) || ((unsigned char)c >= 0x80); }

bool jn_is_valid_id_start(char c) {
  // NOTE: https://www.cprogramming.com/tutorial/unicode.html
  return isalpha(c) || ((unsigned char)c >= 0xC0);
}

int jn_lex(const char *source, int length, int max_out_tokens, JN_Token *out_tokens) {
  int num_tokens = 0;
  for (int i = 0; i < length; ++i) {
    char c = source[i];
    switch (c) {
    case ' ': {
      break;
    }
    default:
      if (jn_is_valid_id_start(c)) {
        jn_lex_id(&source[i], length - i, &out_tokens[num_tokens]);
        i = i + out_tokens[num_tokens].id_length;
        num_tokens++;
      }
      break;
    }
  }
  return num_tokens;
}

void jn_lex_id(const char *source, int length, JN_Token *out_token) {
  out_token->id = source;
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
