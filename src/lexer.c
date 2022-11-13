#include "lexer.h"

#include <ctype.h>
#include <stdio.h>

void jn_lex(JN_Lexer *lexer) {
  int line = 1;
  int column = 1;

  // Iterate until length - 1 to save room for END token.
  while (lexer->current < lexer->length - 1) {
    JN_TokenType type = JN_TT_UNKNOWN;
    int length = 1;

    char c = lexer->source[lexer->current];
    switch (c) {
    case '-': {
      type = JN_TT_DASH;
      break;
    }
    case '=': {
      type = JN_TT_EQUAL;
      break;
    }
    case ';': {
      type = JN_TT_SEMICOLON;
      break;
    }
    case '\n':
      line++;
      column = 0;
    case ' ':
    case '\t':
      goto lex_next;
    default:
      // Identifier
      // NOTE: https://www.cprogramming.com/tutorial/unicode.html
      if (isalpha(c) || ((unsigned char)c >= 0xC0)) {
        type = JN_TT_IDENTIFIER;
        while (lexer->current + length < lexer->length) {
          c = lexer->source[lexer->current + length];
          if (isalpha(c) || isdigit(c) || ((unsigned char)c >= 0x80)) {
            length++;
          } else {
            break;
          }
        }
        break;
      }

      // Integer
      if (isdigit(c)) {
        type = JN_TT_INTEGER;
        while (lexer->current + length < lexer->length) {
          if (isdigit(lexer->source[lexer->current + length])) {
            length++;
          } else {
            break;
          }
        }
        break;
      }
    }

    lexer->tokens[lexer->num_tokens++] = (JN_Token){
      .type = type,
      .line = line,
      .column = column,
      .characters = &lexer->source[lexer->current],
      .length = length,
    };

  lex_next:
    lexer->current += length;
    column += length;
  }

  lexer->tokens[lexer->num_tokens++] = (JN_Token){
    .type = JN_TT_END,
    .line = line,
    .column = column,
    .characters = NULL,
    .length = 1,
  };
}

void jn_print_lexer(JN_Lexer lexer, bool print_tokens) {
  printf("+------------+\n");
  printf("| Lexer      |\n");
  printf("+------------+\n");
  printf("  num_tokens: %d\n", lexer.num_tokens);

  if (!print_tokens)
    return;

  printf("\nTokens:\n");
  for (int i = 0; i < lexer.num_tokens; ++i) {
    printf("  ");
    jn_print_token(lexer.tokens[i]);
  }
}

void jn_print_token(JN_Token token) {
  if (token.type == JN_TT_IDENTIFIER || token.type == JN_TT_INTEGER || token.type == JN_TT_ERROR) {
    printf("%s(", jn_token_type_names[token.type]);
    for (int i = 0; i < token.length; ++i) {
      printf("%c", token.characters[i]);
    }
    printf(", %d)", token.length);
  } else {
    printf("%s\t", jn_token_type_names[token.type]);
  }
  printf("\t[%d, %d]\n", token.line, token.column);
}
