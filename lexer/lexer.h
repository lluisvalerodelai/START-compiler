#ifndef LEXER_HEAD
#define LEXER_HEAD

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
typedef enum {
  identifier,
  keyword,
  separator,
  operator_token,
  literal,
  comment,
  string,
  END,
} token_type;

typedef struct {
  token_type type;
  char *value;
} Token;

char peek_next_char(FILE *file);

Token *lexer(FILE *fptr);

void lexer_tester(int argc, char **argv);

#ifdef __cplusplus
}
#endif
#endif
