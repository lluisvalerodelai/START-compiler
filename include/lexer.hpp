#pragma once
#include <cctype>
#include <csignal>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <vector>

enum class TokenType {
  LeftParen,        // (
  RightParen,       // )
  LeftBrace,        // {
  RightBrace,       // }
  Semicolon,        // ;
  And,              // &
  Or,               // |
  Plus,             // +
  Minus,            // -
  Multiply,         // *
  Divide,           // /
  Equals,           // =
  Not,              // !
  LessThan,         // <
  GreaterThan,      // >
  LessThanEqual,    // <=
  GreaterThanEqual, // >=
  NotEqual,         // !=
  EqualComparison,  // ==
  String,
  Keyword,           // Keywords: int, float, char, string, main, if, while
  Identifier,        // User-defined names
  Integer,           // Ints
  Float,             // floats
  SinglelineComment, //
  MultiLineComment,  // $ this is a multi \n line comment $
  Unknown,           // Unknown token
};

struct Token {
  TokenType type;
  std::string value;
  int line;
  int char_pos;

  Token(TokenType t, const std::string &v, int l, int c)
      : type(t), value(v), line(l), char_pos(c) {}
};

class Lexer {
private:
  std::unordered_set<std::string> keywords = {
      "int", "float", "string", "main", "if", "while", "return", "print"};
  std::unordered_set<char> separators = {'(', ')', '{', '}', ';', '&', '|', '!',
                                         '+', '-', '*', '/', '=', '<', '>'};

  std::unordered_set<std::string> comparison_tokens = {"==", "<=", ">=", "!="};

  bool isKeyword(const std::string &str) {
    return keywords.find(str) != keywords.end();
  }

  bool isSeparator(char ch) { return separators.find(ch) != separators.end(); }

  bool isComparison(std::string s) {
    return comparison_tokens.find(s) != comparison_tokens.end();
  }

  bool isIdentifierStart(char ch) { return std::isalpha(ch) || ch == '_'; }

  bool isIdentifierChar(char ch) { return std::isalnum(ch) || ch == '_'; }

  bool isNumberChar(char ch) { return std::isdigit(ch); }

  bool isStringStart(char ch) { return ch == '"'; }

  TokenType getSeparatorTokenType(char ch);
  TokenType getComparisonTokenType(std::string s);

public:
  std::vector<Token> tokenize(const std::string &input);
};

void printTokens(const std::vector<Token> &tokens);
