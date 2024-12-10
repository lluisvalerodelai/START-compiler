#include "../include/parser.hpp"
#include "../include/lexer.hpp"
#include <cmath>
#include <iostream>
#include <stdexcept>

// Utility to get the current token
Token Parser::currentToken() {
  if (current < tokens.size())
    return tokens[current];
  return {TokenType::Unknown, "", -1, -1};
}

Token Parser::peekToken() {
  if (current < tokens.size()) {
    return tokens[current];
  } else {
    return tokens[current + 1];
  }
}

// Utility to move to the next token
void Parser::nextToken() {
  if (current < tokens.size())
    ++current;
}

// Match a specific token type, otherwise throw an error
void Parser::match(TokenType expected, std::string action) {
  if (currentToken().type == expected) {
    nextToken();
  } else {

    if (action == "") {
      throw std::runtime_error(
          "Syntax Error: Expected " + tokenTypeToString(expected) +
          " but found token of type: " +
          tokenTypeToStringLITERAL(currentToken().type) +
          " with value: " + currentToken().value + " at line " +
          std::to_string(currentToken().line) + ", position " +
          std::to_string(currentToken().char_pos));
    } else {
      throw std::runtime_error(
          "Syntax Error: Expected " + tokenTypeToString(expected) +
          " but found token of type: " +
          tokenTypeToStringLITERAL(currentToken().type) +
          " with value: " + currentToken().value + " at line " +
          std::to_string(currentToken().line) + ", position " +
          std::to_string(currentToken().char_pos) + " while parsing for " +
          action);
    }
  }
}

// Match a specific token type, otherwise throw an error
void Parser::matchValue(std::string expected, std::string action) {
  if (currentToken().value == expected) {
    nextToken();
  } else {

    if (action == "") {
      throw std::runtime_error(
          "Syntax Error: Expected " + expected +
          " but found token of value: " + currentToken().value + " at line " +
          std::to_string(currentToken().line) + ", position " +
          std::to_string(currentToken().char_pos));
    } else {
      throw std::runtime_error(
          "Syntax Error: Expected " + expected +
          " but found token of value: " + currentToken().value + " at line " +
          std::to_string(currentToken().line) + ", position " +
          std::to_string(currentToken().char_pos) + " while parsing for " +
          action);
    }
  }
}

std::string Parser::tokenTypeToStringLITERAL(TokenType type) {
  switch (type) {
  case TokenType::LeftParen:
    return "LeftParen";
  case TokenType::RightParen:
    return "RightParen";
  case TokenType::LeftBrace:
    return "LeftBrace";
  case TokenType::RightBrace:
    return "RightBrace";
  case TokenType::Semicolon:
    return "Semicolon";
  case TokenType::Keyword:
    return "Keyword";
  case TokenType::Identifier:
    return "Identifier";
  case TokenType::Integer:
    return "Integer";
  case TokenType::Float:
    return "Float";
  case TokenType::String:
    return "String";
  case TokenType::Not:
    return "Logical Not";
  case TokenType::And:
    return "Logical And";
  case TokenType::Or:
    return "Logical Or";

  default:
    return "Unknown";
  }
}

// Convert TokenType to string for error messages
std::string Parser::tokenTypeToString(TokenType type) {
  switch (type) {
  case TokenType::LeftParen:
    return "(";
  case TokenType::RightParen:
    return ")";
  case TokenType::LeftBrace:
    return "{";
  case TokenType::RightBrace:
    return "}";
  case TokenType::Semicolon:
    return ";";
  case TokenType::Keyword:
    return "Keyword";
  case TokenType::Identifier:
    return "Identifier";
  case TokenType::Integer:
    return "Integer";
  case TokenType::Float:
    return "Float";
  case TokenType::String:
    return "String";
  case TokenType::Not:
    return "!";
  case TokenType::And:
    return "&";
  case TokenType::Or:
    return "|";
  default:
    return "Unknown";
  }
}

// Parsing functions for each non-terminal
ASTNode *Parser::program() {
  match(TokenType::Keyword);           // "int"
  matchValue("main", "main function"); // main
  match(TokenType::LeftParen);         // "("
  match(TokenType::RightParen);        // ")"
  match(TokenType::LeftBrace);         // "{"
  ASTNode *stmtList = statements();    // get the statements
  match(TokenType::RightBrace);        // "}"
  return stmtList;
}

// return a statements node
ASTNode *Parser::statements() {
  ASTNode *statements_node = new ASTNode("statements");

  // keep parsing statements until we reach closing { of main()
  while (currentToken().type != TokenType::RightBrace) {
    ASTNode *child = statement();
    statements_node->children.emplace_back(child);
  }

  return statements_node;
}

// parse and return a statement node
ASTNode *Parser::statement() {

  // check if we are looking at a declaration
  if (currentToken().type == TokenType::Keyword) {
    if (currentToken().value == "int" || currentToken().value == "float" ||
        currentToken().value == "string") {

      ASTNode *statement_node = declaration();
      return statement_node;
    }
  }

  // check if we are looking at an assignment
  if (currentToken().type == TokenType::Identifier) {
    ASTNode *assignment_node = assignment();
    return assignment_node;
  }

  // check if we are looking at an if
  if (currentToken().value == "if") {
    ASTNode *if_node = if_statement();
    return if_node;
  }

  // check if we are looking at a while
  if (currentToken().value == "while") {
    ASTNode *while_node = while_statement();
    return while_node;
  }

  // if none of those, then throw error
  throw std::runtime_error("failed to parse statement???????");
}

ASTNode *Parser::declaration() {
  // declaration node has children: identifier, expression
  ASTNode *declaration_node = new ASTNode("declaration");

  // we know it will be either int, float, or string since its only called if
  // these match
  match(TokenType::Keyword, "declaration");

  match(TokenType::Identifier, "declaration");

  match(TokenType::Equals, "declaration");

  // should actually be:
  // ASTNode *expression_node = expression();
  //->expression would then parse an expression
  ASTNode *expression_node = expression();
  declaration_node->children.emplace_back(expression_node);

  match(TokenType::Semicolon, "variable declaration");

  return declaration_node;
}

ASTNode *Parser::if_statement() {
  ASTNode *if_node = new ASTNode("if node");

  matchValue("if");
  match(TokenType::LeftParen, "if statement");  //(
  match(TokenType::RightParen, "if statement"); //)
  match(TokenType::LeftBrace, "if statement");  //{
  if_node->children.emplace_back(statements());
  match(TokenType::RightBrace, "if statement"); //}

  return if_node;
};

ASTNode *Parser::while_statement() {
  ASTNode *while_node = new ASTNode("while node");

  matchValue("while");
  match(TokenType::LeftParen, "while statement");  //(
  match(TokenType::RightParen, "while statement"); //)
  match(TokenType::LeftBrace, "while statement");  //{
  while_node->children.emplace_back(statements());
  match(TokenType::RightBrace, "while statement"); //}

  return while_node;
};

ASTNode *Parser::assignment() {
  //<assignment>    ::= <identifier> "=" <expression> ";"
  ASTNode *assignemnt_node = new ASTNode("assignemnt node");

  match(TokenType::Identifier, "identifier of assignment statement");

  match(TokenType::Equals, "assignemnt statement");

  ASTNode *expression_node = expression();
  assignemnt_node->children.emplace_back(expression_node);

  match(TokenType::Semicolon, "assignemnt statement");

  return assignemnt_node;
}

// Updated expression parsing with proper operator precedence
ASTNode *Parser::expression() { return parseOR(); }

ASTNode *Parser::parseOR() {
  ASTNode *left = parseAND();

  while (currentToken().type == TokenType::Or) { // Handle logical OR
    Token op = currentToken();
    nextToken();
    ASTNode *right = parseAND();
    ASTNode *node = new ASTNode(op.value);
    node->children.emplace_back(left);
    node->children.emplace_back(right);
    left = node;
  }

  return left;
}

ASTNode *Parser::parseAND() {
  ASTNode *left = parseUnary();

  while (currentToken().type == TokenType::And) { // Handle logical AND
    Token op = currentToken();
    nextToken();
    ASTNode *right = parseUnary();
    ASTNode *node = new ASTNode(op.value);
    node->children.emplace_back(left);
    node->children.emplace_back(right);
    left = node;
  }

  return left;
}

ASTNode *Parser::parseUnary() {
  if (currentToken().type == TokenType::Not) { // Handle NOT
    Token op = currentToken();
    nextToken();
    ASTNode *operand = parseUnary();
    ASTNode *node = new ASTNode(op.value);
    node->children.emplace_back(operand);
    return node;
  }

  if (currentToken().type == TokenType::Minus) { // Handle negation
    Token op = currentToken();
    nextToken();
    ASTNode *operand = parseUnary();
    ASTNode *node = new ASTNode(op.value);
    node->children.emplace_back(operand);
    return node;
  }

  return parseTerm();
}

ASTNode *Parser::parseTerm() {
  ASTNode *left = parseFactor(); // returned a 4

  while (currentToken().type == TokenType::Plus ||
         currentToken().type == TokenType::Minus) { // Handle +/- last
    Token op = currentToken();
    nextToken();
    ASTNode *right = parseFactor();
    ASTNode *node = new ASTNode(op.value);
    node->children.emplace_back(left);
    node->children.emplace_back(right);
    left = node;
  }

  return left;
}

ASTNode *Parser::parseFactor() {

  ASTNode *left = parseNumeric();

  while (currentToken().type == TokenType::Multiply ||
         currentToken().type == TokenType::Divide) {
    Token op = currentToken();
    nextToken();
    ASTNode *right = parseNumeric();
    ASTNode *node = new ASTNode(op.value);
    node->children.emplace_back(left);
    node->children.emplace_back(right);
    left = node;
  }

  return left;
}

ASTNode *Parser::parseNumeric() {
  if (currentToken().type ==
      TokenType::LeftParen) { // Handle parenthetical expressions
    nextToken();
    ASTNode *expr = expression();
    match(TokenType::RightParen);
    return expr;
  }

  if (currentToken().type == TokenType::Integer ||
      currentToken().type == TokenType::Float) {
    ASTNode *node = new ASTNode(currentToken().value);
    nextToken();
    return node;
  }

  if (currentToken().type == TokenType::Identifier) {
    ASTNode *node = new ASTNode(currentToken().value);
    nextToken();
    return node;
  }

  throw std::runtime_error("Syntax Error: Invalid factor.");
}

void printAST(ASTNode *node, int depth = 0) {

  if (!node)
    return;

  std::cout << std::string(depth * 2, ' ') << node->value << "\n";

  for (ASTNode *child : node->children) {
    printAST(child, depth + 1);
  }
}

int main() {
  std::string raw_input = R"(int main() {

  int a = 3 + (4 + (5*6));
  float pi = 3;

  if () {

  b = 25 * 13;
    }
  while () {

  int b = 0 | (1 & 0);
    }


 })";

  Lexer lexer;
  // Replace this with tokens from your lexer
  std::vector<Token> tokens = lexer.tokenize(raw_input);
  printTokens(tokens);
  try {

    Parser parser(tokens);
    ASTNode *ast = parser.parse();

    printAST(ast);
    delete ast;

  } catch (const std::exception &e) {
    std::cerr << e.what() << "\n";
  }

  return 0;
}
