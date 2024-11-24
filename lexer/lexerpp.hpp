#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <istream>

enum token_type {
  identifier,
  keyword,
  separator,
  operator_token,
  literal,
  END,
  comment,
};

struct Token {
  token_type type;
  std::string value;
  int characterPosition;

  Token(token_type t, const std::string &v, int pos)
      : type(t), value(v), characterPosition(pos) {}
};

/// @brief Allows us to call lexer with either the content of a file, or by using a file stream
/// @tparam Elem 
/// @tparam Traits 
/// @param istream 
/// @return 
template <class Elem, class Traits>
std::vector<Token> lexer(std::basic_istream<Elem, Traits>& istream);

std::vector<Token> lexer(const std::string& file_name);
