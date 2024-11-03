#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
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
