#ifndef LEXER_HEAD
#define LEXER_HEAD

typedef enum {
  identifier,
  keyword,
  separator,
  operator,
  literal,
  comment,
  string,
  END,
} token_type;

typedef struct {
  token_type type;
  char *value;
} Token;

#endif
