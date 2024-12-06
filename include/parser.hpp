#pragma once
#include "lexer.hpp"
#include <stack>

class Parser {

private:
  const std::vector<Token> m_tokens;
  std::stack<Token> m_tokenStack;

public:

  bool ParseVarAssignment();

  Parser(std::vector<Token> t) : m_tokens(t) {
    // build the token_stack
    for (int i = t.size() - 1; i >= 0; i--) {
      m_tokenStack.push(t[i]);
    }
  };

  ~Parser() = default;
};
