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
};

struct Token {
  token_type type;
  std::string value;
};
