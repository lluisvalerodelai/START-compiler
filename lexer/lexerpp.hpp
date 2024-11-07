#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
};

std::vector<Token> lexer(std::ifstream &F);
void pretty_print_tokens(std::vector<Token> tokens);
