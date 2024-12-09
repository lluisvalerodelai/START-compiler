#pragma once
#include "../include/lexer.hpp"

struct ASTNode {
  std::string value;
  std::vector<ASTNode *> children;

  ASTNode(const std::string &val) : value(val) {}
  ~ASTNode() {
    for (ASTNode *child : children) {
      delete child;
    }
  }
};

class Parser {
private:
  std::vector<Token> tokens;
  size_t current = 0;

  // Utility to get the current token
  Token currentToken();

  // Utility to move to the next token
  void nextToken();

  // Match a specific token type, otherwise throw an error
  void match(TokenType expected, std::string action = "");

  // Match a specific token type, otherwise throw an error
  void matchValue(std::string expected, std::string action = "");

  std::string tokenTypeToStringLITERAL(TokenType type);

  // Convert TokenType to string for error messages
  std::string tokenTypeToString(TokenType type);

  // Parsing functions for each non-terminal
  ASTNode *program();

  // return a statements node
  ASTNode *statements();

  // parse and return a statement node
  ASTNode *statement();

  ASTNode *declaration();

  ASTNode *if_statement();

  ASTNode *while_statement();

  ASTNode *assignment();

public:
  Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

  ASTNode *parse() { return program(); }
};
