#include <iostream>
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>
#include <variant>

/*
 *
 * Defined tokens:
 *  -separators: ( ) { } ; " "
 *  -operators: + - * / ! || &&
 *  -keywords: int, float, char, return, print, if, else, while, for
 *  -identifier specs: can contain '_' but no '-' also can not start with numbers
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
    const int line;
    const int line_char;
    const int glob_char;

    Position()
      : line(0), line_char(0), glob_char(0) {}
    Position(int l, int lc, int gc)
      : line(l), line_char(lc), glob_char(gc) {}
  };
  struct Range {
    const Position start;
    const Position end;

    Range()
      : start(), end() {}
    Range(const Position& s, const Position& e)
      : start(s), end(e) {}
  };

  const std::variant<Position, Range> location;
  const token_type type;
  const std::string content;

  Token(const std::variant<Position, Range>& l, token_type t, const std::string& c)
    : location(l), type(t), content(c) {}

  ~Token() = default;
};

class Lexer {
private:
  std::string processed_string;
  std::vector<Token> processed_tokens;
public:
   std::vector<Token> lex(const std::string& raw_input);

  ~Lexer() = default;
};