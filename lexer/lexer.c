#include "lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum token_type;

struct Token;

char peek_next_char(FILE *file) {
  // Save the current position
  long current_pos = ftell(file);

  // Get the next character
  char next_char = fgetc(file);

  // Restore the original position
  fseek(file, current_pos, SEEK_SET);

  return next_char; // Return the character that was peeked
}

Token *lexer(FILE *fptr) {

  if (fptr == NULL) {
    printf("\e[31m ERROR \e[0m: file could not be found \n");
  }

  Token end_token = {.type = END, .value = "\0"};

  int num_tokens = 0;
  int tokens_size = 25;

  Token *tokens = malloc(sizeof(Token) * (tokens_size + 1));
  if (tokens == NULL) {
    printf("\e[31m ERROR \e[0m: Failed to allocate mem for tokens \n");
  }

  tokens[tokens_size] = end_token;

  char c;
  bool are_checking_string = false;

  while (c != EOF) {

    if (num_tokens > tokens_size) {
      tokens_size = tokens_size + 25;
      tokens = realloc(tokens, sizeof(Token) * (tokens_size + 1));
      tokens[tokens_size] = end_token;
    }

    c = fgetc(fptr);
    if (!are_checking_string) {
      switch (c) {
      case '\b':
        continue;
      case '\f':
        continue;
      case '\n':
        continue;
      case ' ':
        continue;
      case '\t':
        continue;
      case '\e':
        continue;
      }
    }
    if (c == '{') {
      Token obrace = {.type = separator, .value = "{"};
      tokens[num_tokens] = obrace;
      num_tokens++;
    } else if (c == '}') {
      Token cbrace = {.type = separator, .value = "}"};
      tokens[num_tokens] = cbrace;
      num_tokens++;
    } else if (c == '(') {
      Token oparen = {.type = separator, .value = "("};
      tokens[num_tokens] = oparen;
      num_tokens++;
    } else if (c == ')') {
      Token cparen = {.type = separator, .value = ")"};
      tokens[num_tokens] = cparen;
      num_tokens++;
    } else if (c == ';') {
      Token sc = {.type = separator, .value = ";"};
      tokens[num_tokens] = sc;
      num_tokens++;
    } else if (c == '=') {
      Token e = {.type = separator, .value = "="};
      tokens[num_tokens] = e;
      num_tokens++;
    } else if (c == '+') {
      Token e = {.type = separator, .value = "+"};
      tokens[num_tokens] = e;
      num_tokens++;
    } else if (c == '-') { // still doesent work
      char next_c = peek_next_char(fptr);
      if (next_c == '>') {
        Token e = {.type = operator, .value = "->" };
        tokens[num_tokens] = e;
        num_tokens++;
      } else {
        Token e = {.type = operator, .value = "-" };
        tokens[num_tokens] = e;
        num_tokens++;
      }

    } else if (c >= '0' && c <= '9') {
      char *buffer = malloc((sizeof(char) * 10) + 1);
      if (buffer == NULL) {
        printf(" \e[31m ERROR \e[0m: Null pointer when allocating for "
               "numerical token\n");
        exit(1);
      }
      int i = 0;
      while (c >= '0' && c <= '9' || c == '.') {
        if ((i + 1) % 10 == 0) {
          char *buffer = realloc(buffer, (sizeof(char) * 10) * i);
          if (buffer == NULL) {
            printf(" \e[31m ERROR \e[0m: Null pointer when re-allocating for "
                   "numerical token\n");
            exit(1);
          }
        }
        buffer[i] = c;
        i++;
        char next_c = peek_next_char(fptr);
        if (next_c >= '0' && next_c <= '9' || next_c == '.') {
          c = fgetc(fptr);
        } else {
          break;
        }
      }
      Token numeric = {.type = literal, .value = buffer};
      tokens[num_tokens] = numeric;
      num_tokens++;
    } else if (c == '"') {
      are_checking_string = true;
      int i = 1;
      char *buffer = malloc((sizeof(char) * 10) + 4);

      buffer[0] = c;

      c = fgetc(fptr);

      while (c != '"') {
        buffer[i] = c;
        if ((i % 10) == 0) {
          char *temp_buffer = realloc(buffer, (i + 10) * sizeof(char));
          buffer = temp_buffer;
        }
        c = fgetc(fptr);
        i++;
      }
      buffer[i] = '"';
      Token string_token = {.type = string, .value = buffer};
      tokens[num_tokens] = string_token;
      num_tokens++;
      are_checking_string = false;

    } else {
      int i = 1;
      char *buffer = malloc((sizeof(char) * i) + 10);
      while (isalnum(c) && c != EOF || c == '_' || c == '-' || c == ':' ||
             c == '=') {
        buffer[i - 1] = c;
        i++;
        if (i % 10 == 0) {
          buffer = realloc(buffer, sizeof(char) * i);
        }
        char next_c = peek_next_char(fptr);
        if (isalnum(next_c) || next_c == '_' || next_c == '-' ||
            next_c == ':' || next_c == '=') {
          c = fgetc(fptr);
        } else {
          break;
        }
      }
      if (strcmp(buffer, "var") == 0) {
        Token identifier_token = {.type = keyword, .value = buffer};
        tokens[num_tokens] = identifier_token;
        num_tokens++;
      } else if (strcmp(buffer, "is") == 0) {
        Token identifier_token = {
            .type = keyword, .value = buffer}; // left as keyword for now, might
        tokens[num_tokens] = identifier_token; // need to be changed to operator
        num_tokens++;
      } else if (strcmp(buffer, "char") == 0) {
        Token identifier_token = {.type = keyword, .value = buffer};
        tokens[num_tokens] = identifier_token;
        num_tokens++;
      } else if (strcmp(buffer, "number") == 0) {
        Token identifier_token = {.type = keyword, .value = buffer};
        tokens[num_tokens] = identifier_token;
        num_tokens++;
      } else if (strcmp(buffer, "string") == 0) {
        Token identifier_token = {.type = keyword, .value = buffer};
        tokens[num_tokens] = identifier_token;
        num_tokens++;
      } else if (strcmp(buffer, "arguments") == 0) {
        Token identifier_token = {.type = keyword, .value = buffer};
        tokens[num_tokens] = identifier_token;
        num_tokens++;
      } else if (strcmp(buffer, ":=") == 0) {
        Token operator_token = {.type = operator, .value = buffer };
        tokens[num_tokens] = operator_token;
        num_tokens++;
      } else {
        Token identifier_token = {.type = identifier, .value = buffer};
        tokens[num_tokens] = identifier_token;
        num_tokens++;
      }
    }
  }
  return tokens;
}

int main(int argc, char **argv) {

  if (argc != 2) {
    printf(" \e[31m ERROR \e[0m: LEXER accepts only 1 argument -> FILENAME \n");
    return 1;
  }
  FILE *fptr = fopen(argv[1], "r");

  if (fptr == NULL) {
    printf(" \e[31m ERROR \e[0m: File could not be found");
  }

  Token *tokens = lexer(fptr);
  printf("Tokens \n");
  int i = 0;
  while (tokens[i].type != END) {
    printf("%i \t %i \t %s\n", i, tokens[i].type, tokens[i].value);
    i++;
  }
  return 0;
}
