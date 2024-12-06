#pragma once
#include "lexer.hpp"

class Parser {

private:
  const std::vector<Token> m_tokens;
  mutable int m_tokenIndex;
  std::vector<std::string> m_errors;

  bool ParseVarAssignment();
  bool ParseVarDec();
  bool ParseExpression(int curr_index = -1);

  std::string AddError(std::string message, Token bad_token); 

public:

  bool ParseProgram();

  Parser(std::vector<Token> t) : m_tokens(t) {};

  ~Parser() = default;
};
