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

  // Utility to peek the next token
  Token peekToken();

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

  //parse and return a declaration node
  ASTNode *declaration();

  //parse and return an if statement node
  ASTNode *if_statement();

  //parse and return a while statement node
  ASTNode *while_statement();

//parse and return an assignment node 
  ASTNode *assignment();

  //parse and return an expression node 
  ASTNode *expression();

  //parse and return a term node 
  ASTNode *parseTerm();

  //parse and return a factor node 
  ASTNode *parseFactor();

  //parse and return a factor node 
  ASTNode *parseNumeric();

  //parse and return a factor node 
  ASTNode *parseOR();
  //parse and return a factor node 
  ASTNode *parseAND();
  //parse and return a factor node 
  ASTNode *parseUnary();

public:
  Parser(const std::vector<Token> &tokens) : tokens(tokens) {}

  ASTNode *parse() { return program(); }
};
