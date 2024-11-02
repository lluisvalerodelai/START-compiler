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
  comment,
  END,
};

struct Token {
  token_type type;
  char *value;
};
