#include "lexer.hpp"
#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

/*
 *
 * Defined tokens:
 *  -separators: ( ) { } ; " "
 *  -operators: + - * / ! || &&
 *  -keywords: int, float, char, return, print, if, else, while, for
 *
 */

enum token_type {
  l_paren,
  r_paren,
  l_brace,
  r_brace,
  semicolon,
  quotes,
  add_op,
  minus_op,
  times_op,
  divide_op,
  or_op,
  and_op,
  int_keyword,
  float_keyword,
  char_keyword,
  return_keyword,
  print_keyword,
  if_keyword,
  else_keyword,
  while_keyword,
  for_keyword,
};

class Token {
public:
  struct Position {
    int line_nr;
    int local_character_number;
    int global_character_number;
  };

  Position position;
  const token_type type;

  Token(Position p, token_type t) : position(p), type(t) {}

  ~Token() = default;
};

class Lexer {
public:
  mutable std::string file_string;
  mutable std::vector<Token *> token_list;

  std::vector<std::unique_ptr<Token>> lex(std::string raw_input) {

    file_string = raw_input;
    std::vector<std::unique_ptr<Token>> tokens;

    return tokens;
  }

  ~Lexer() = default;
};

int main(int argc, char **argv) {

  std::string example_string = "hello world";

  Lexer lex;
  lex.file_string = example_string;

  std::vector<Token *> tokens = lex.lex(example_string);

  std::cout << tokens[0]->type;

  return 0;
}
