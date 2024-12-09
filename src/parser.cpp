#include "../include/parser.hpp"
#include "../include/lexer.hpp"
#include <iostream>
#include <stdexcept>

// Utility to get the current token
Token Parser::currentToken() {
  if (current < tokens.size())
    return tokens[current];
  return {TokenType::Unknown, "", -1, -1};
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
  default:
    return "Unknown";
  }
}

// Parsing functions for each non-terminal
ASTNode *Parser::program() {
  match(TokenType::Keyword);        // "int"
  match(TokenType::Identifier);     // "main"
  match(TokenType::LeftParen);      // "("
  match(TokenType::RightParen);     // ")"
  match(TokenType::LeftBrace);      // "{"
  ASTNode *stmtList = statements(); // get the statements
  match(TokenType::RightBrace);     // "}"
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
  match(TokenType::Keyword, "type of variable that is being declared");

  match(TokenType::Identifier, "identifier of varaible being declared");

  match(TokenType::Equals, "equal signs for variable being declared");

  // should actually be:
  // ASTNode *expression_node = expression();
  //->expression would then parse an expression
  ASTNode *expression_node = new ASTNode("expression_value");
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
  match(TokenType::RightBrace, "if statement"); //}

  if_node->children.emplace_back(new ASTNode("statement_blocl"));
  return if_node;
};

ASTNode *Parser::while_statement() {
  ASTNode *if_node = new ASTNode("while node");

  matchValue("while");
  match(TokenType::LeftParen, "while statement");  //(
  match(TokenType::RightParen, "while statement"); //)
  match(TokenType::LeftBrace, "while statement");  //{
  match(TokenType::RightBrace, "while statement"); //}

  if_node->children.emplace_back(new ASTNode("statement_blocl"));
  return if_node;
};

ASTNode *Parser::assignment() {
  //<assignment>    ::= <identifier> "=" <expression> ";"
  ASTNode *assignemnt_node = new ASTNode("assignemnt node");

  match(TokenType::Identifier, "identifier of assignment statement");

  match(TokenType::Equals, "assignemnt statement");

  ASTNode *expression_node = new ASTNode("expression_value");
  assignemnt_node->children.emplace_back(expression_node);

  match(TokenType::Semicolon, "assignemnt statement");

  return assignemnt_node;
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
  // Replace this with tokens from your lexer
  std::vector<Token> tokens = {{TokenType::Keyword, "int", 1, 1},
                               {TokenType::Identifier, "main", 1, 5},
                               {TokenType::LeftParen, "(", 1, 9},
                               {TokenType::RightParen, ")", 1, 10},
                               {TokenType::LeftBrace, "{", 1, 12},

                               {TokenType::Keyword, "int", 2, 2},
                               {TokenType::Identifier, "x", 2, 6},
                               {TokenType::Equals, "=", 2, 8},
                               {TokenType::Semicolon, ";", 2, 11},
                               /* {TokenType::Integer, "5", 2, 10}, */

                               {TokenType::Identifier, "z", 5, 6},
                               {TokenType::Equals, "=", 5, 8},
                               {TokenType::Semicolon, ";", 5, 11},
                               /* {TokenType::Integer, "5", 2, 10}, */

                               {TokenType::Keyword, "float", 3, 2},
                               {TokenType::Identifier, "y", 3, 6},
                               {TokenType::Equals, "=", 3, 8},
                               {TokenType::Semicolon, ";", 3, 11},
                               /* {TokenType::Integer, "5", 2, 10}, */

                               {TokenType::Keyword, "if", 1, 2},
                               {TokenType::LeftParen, "(", 1, 2},
                               {TokenType::RightParen, ")", 1, 2},
                               {TokenType::LeftBrace, "{", 5, 2},
                               {TokenType::RightBrace, "}", 5, 2},

                               {TokenType::Keyword, "string", 4, 2},
                               {TokenType::Identifier, "z", 4, 6},
                               {TokenType::Equals, "=", 4, 8},
                               {TokenType::Semicolon, ";", 4, 11},
                               /* {TokenType::Integer, "5", 2, 10}, */

                               {TokenType::Keyword, "while", 4, 8},
                               {TokenType::LeftParen, "(", 4, 11},
                               {TokenType::RightParen, ")", 4, 11},
                               {TokenType::LeftBrace, "{", 4, 11},
                               {TokenType::RightBrace, "}", 4, 11},

                               {TokenType::Identifier, "z", 5, 6},
                               {TokenType::Equals, "=", 5, 8},
                               {TokenType::Semicolon, ";", 5, 11},
                               /* {TokenType::Integer, "5", 2, 10}, */

                               {TokenType::RightBrace, "}", 6, 1}};

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
