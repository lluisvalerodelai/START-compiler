#include "../include/parser.hpp"
#include "../include/lexer.hpp"
#include <pthread.h>
#include <string>
#include <vector>

bool Parser::ParseVarAssignment() {
  //<var_assign> ::= identifier + "=" + expression

  // check that a variable assignment would even fit with the amt of tokens left
  if (m_tokens.size() - m_tokenIndex < 3) {
    return false;
  }

  int peek_i = m_tokenIndex;

  // do we find an identifier

  if (m_tokens[peek_i].type != TokenType::Identifier) {
    return false;
  }

  // do we find an =
  peek_i += 1;
  if (m_tokens[peek_i].value != "=") {
    return false;
  }

  // do we find an acceptable type
  peek_i += 1;
  if (!ParseExpression(peek_i)) {
    return false;
  }

  m_tokenIndex = peek_i + 1;
  return true;
}

bool Parser::ParseVarDec() {
  //"int" | "float" | "char" + identifier + "=" + [expression>]
  if (m_tokens.size() - m_tokenIndex < 4) {
    return false;
  }

  int peek_i = m_tokenIndex;

  if (m_tokens[peek_i].value != "int" && m_tokens[peek_i].value != "float" &&
      m_tokens[peek_i].value != "string") {
    return false;
  }

  peek_i += 1;
  if (m_tokens[peek_i].type != TokenType::Identifier) {
    return false;
  }

  peek_i += 1;
  if (m_tokens[peek_i].value != "=") {
    return false;
  }

  peek_i += 1;
  if (!ParseExpression(peek_i)) {
    return false;
  }

  m_tokenIndex = peek_i + 1;
  return true;
}

bool Parser::ParseExpression(int curr_index) {
  //<expression> ::= int | float | char
  int peek_i;

  if (curr_index == -1) {
    peek_i = m_tokenIndex;
  } else {
    peek_i = curr_index;
  }

  if (m_tokens.size() - peek_i < 1) {
    return false;
  }

  Token curr_tok = m_tokens[peek_i];

  if (curr_tok.type != TokenType::Integer &&
      curr_tok.type != TokenType::Float && curr_tok.type != TokenType::String) {
    return false;
  }

  m_tokenIndex = peek_i + 1;
  return true;
}

bool Parser::ParseProgram() {
  m_tokenIndex = 0;

  while (m_tokens.size() - m_tokenIndex > 0) {
    std::cout << "------------curr pos: " << m_tokenIndex << "-----------"
              << std::endl;

    Token curr_tok = m_tokens[m_tokenIndex];

    if (curr_tok.type == TokenType::Unknown) {
      AddError("Unkown token value", curr_tok);
      // go to the next token and keep parsing
      m_tokenIndex += 1;
      continue;
    }

    if (ParseVarAssignment()) {
      std::cout << "\033[1m parsed assignment \033[0m \n";
      continue;
    }
    if (ParseVarDec()) {
      std::cout << "\033[1m parsed variable declaration \033[0m \n";
      continue;
    }
    if (ParseExpression()) {
      std::cout << "\033[1m parsed expression \033[0m \n";
      continue;
    }

    AddError("Unrecognized grammar", curr_tok);
    m_tokenIndex += 1;
  }
  std::cout << "curr pos: " << m_tokenIndex << std::endl;

  std::cout << m_errors.size();

  return true;

};

std::string Parser::AddError(std::string message, Token bad_token) {
  return "\e[31m Error\e[0m on line \e[31m " + std::to_string(bad_token.line) +
         "\e[0m : " + message + "\n";
}

int main() {

  std::string input =
      R"(int a = 3 
    float pi = 3.14 string hello_world2 = "hello world!"
    
    a = 10
    pi = 3.0
  )";

  Lexer lexer;
  std::vector<Token> tokens = lexer.tokenize(input);

  printTokens(tokens);

  Parser P(tokens);
  P.ParseProgram();

  return 0;
}
