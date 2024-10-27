// TODO
//  -redo token type system,
//  -Token -> type, string value (convert numerics to ints in parsing)

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
  identifier,
  keyword,
  separator,
  operator,
  literal,
  comment,
  whitespace,
} token_type;

typedef enum {
  obrace,
  cbrace,
  oparen,
  cparen,
  sc,
  eq,
} value_id;

typedef struct {
  token_type type;
  value_id value;
} Token_tuple;

typedef struct {
  Token_tuple *array;
  int dA_len;
  int len_used;
} dTokenArray;

void init_dArray(dTokenArray *dAp, int initial_size) {
  dAp->dA_len = initial_size;
  dAp->array = malloc((10 + initial_size) * sizeof(Token_tuple));
  dAp->len_used = 0;
}

void insert_dArray(dTokenArray *dAp, Token_tuple token) {
  if (dAp->len_used == dAp->dA_len) {
    dAp->dA_len = dAp->dA_len * 2;
    dAp->array = realloc(dAp->array, dAp->dA_len * sizeof(Token_tuple));
  }
  dAp->len_used++;
  dAp->array[dAp->len_used] = token;
}

void free_dArray(dTokenArray *dAp) {
  free(dAp->array);
  dAp->len_used = 0;
  dAp->dA_len = 0;
  dAp = NULL;
}

void print_dArray(dTokenArray *dAp) {
  for (int i = 0; i < dAp->len_used + 1; i++) {
    Token_tuple token = dAp->array[i];
    printf("%i \t %u \n", i, token.value);
  }
}

char peek_next_char(FILE *file) {
  // Save the current position
  long current_pos = ftell(file);

  // Get the next character
  char next_char = fgetc(file);

  // Restore the original position
  fseek(file, current_pos, SEEK_SET);

  return next_char; // Return the character that was peeked
}

dTokenArray lexer(FILE *fptr) {

  if (fptr == NULL) {
    printf("\e[31m ERROR \e[0m: file could not be found \n");
  }

  dTokenArray TOKENS;
  init_dArray(&TOKENS, 100);

  char c;
  bool are_checking_string = false;

  while (c != EOF) {
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
      Token_tuple token = {.value = obrace, .type = separator};
      insert_dArray(&TOKENS, token);
    } else if (c == '}') {
      Token_tuple token = {.value = cbrace, .type = separator};
      insert_dArray(&TOKENS, token);
    } else if (c == '(') {
      Token_tuple token = {.value = oparen, .type = separator};
      insert_dArray(&TOKENS, token);
    } else if (c == ')') {
      Token_tuple token = {.value = cparen, .type = separator};
      insert_dArray(&TOKENS, token);
    } else if (c == ';') {
      Token_tuple token = {.value = sc, .type = separator};
      insert_dArray(&TOKENS, token);
    } else if (c == '=') {
      Token_tuple token = {.value = eq, .type = separator};
      insert_dArray(&TOKENS, token);
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
      printf("Found numeric token: %s \n", buffer);
    } else if (c == '"') {
      are_checking_string = true;
      int i = 1;
      char *buffer = malloc((sizeof(char) * 10) + 4);

      buffer[0] = c;

      c = fgetc(fptr);

      while (c != '"') {
        buffer[i] = c;
        printf("i = %i\n", i);
        if ((i % 10) == 0) {
          char *temp_buffer = realloc(buffer, (i + 10) * sizeof(char));
          buffer = temp_buffer;
        }

        c = fgetc(fptr);
        i++;
      }
      buffer[i] = '"';
      printf("found string token: %s \n", buffer);
      are_checking_string = false;

    } else {
      int i = 1;
      char *buffer = malloc((sizeof(char) * i) + 1);
      while (isalnum(c) && c != EOF || c == '_' || c == '-') {
        buffer[i - 1] = c;
        i++;
        if (i % 10 == 0) {
          buffer = realloc(buffer, sizeof(char) * i);
        }
        char next_c = peek_next_char(fptr);
        if (isalnum(next_c) || next_c == '_' || next_c == '-') {
          c = fgetc(fptr);
        } else {
          break;
        }
      }

      if (strcmp(buffer, "main") == 0) {
        printf("Found keyword: main \n");
      } else if (strcmp(buffer, "return") == 0) {
        printf("Found keyword: return\n");
      } else if (strcmp(buffer, "int") == 0) {
        printf("Found keyword: int \n");
      } else {
        printf("Found identifier: %s \n", buffer);
      }
    }
  }
  printf(" ----TOKEN LIST---- \n");
  print_dArray(&TOKENS);
  return TOKENS;
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

  lexer(fptr);

  return 0;
}
