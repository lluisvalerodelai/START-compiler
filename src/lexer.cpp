#include "../include/lexer.hpp"

TokenType Lexer::getSeparatorTokenType(char ch) {
  switch (ch) {
  case '(':
    return TokenType::LeftParen;
  case ')':
    return TokenType::RightParen;
  case '{':
    return TokenType::LeftBrace;
  case '}':
    return TokenType::RightBrace;
  case ';':
    return TokenType::Semicolon;
  case '&':
    return TokenType::And;
  case '|':
    return TokenType::Or;
  case '+':
    return TokenType::Plus;
  case '-':
    return TokenType::Minus;
  case '*':
    return TokenType::Multiply;
  case '/':
    return TokenType::Divide;
  case '=':
    return TokenType::Equals;
  default:
    return TokenType::Unknown;
  }
}

std::vector<Token> Lexer::tokenize(const std::string &input) {
  std::vector<Token> tokens;
  int line = 1;
  int char_pos = 1;

  for (size_t i = 0; i < input.size(); i++) {
    char ch = input[i];

    if (ch == '\n') {
      line++;
      char_pos = 1;
      continue;
    }

    if (std::isspace(ch)) {
      char_pos++;
      continue;
    }

    if (ch == '/' && input[i + 1] == '/') {
      // while we either reach a newline or havent hit the end of file
      int start_i = i;
      std::string comment_buf;
      i += 2; // skip the //

      while (i < input.length() && input[i] != '\n') {
        comment_buf += input[i];
        i++;
      }

      tokens.emplace_back(TokenType::SinglelineComment, comment_buf, line,
                          start_i);
      line += 1;
      char_pos = 1;

      continue;
    }

    if (ch == '$') { // Handle string literals

      size_t start = i;
      std::string str_value;
      i++; // move past the first $

      while (i < input.size() && input[i] != '$') {

        if (input[i] == '\n') {
          line += 1;
          char_pos = 1;
        }

        str_value += input[i];
        char_pos++;
        i++;
      }

      if (i < input.size() && input[i] == '$') {
        i++; // Move past the closing quote
        tokens.emplace_back(TokenType::MultiLineComment, str_value, line,
                            char_pos);
        char_pos += (i - start);
      } else {
        // Unterminated multi-line comment
        tokens.emplace_back(TokenType::Unknown, input.substr(start), line,
                            char_pos);
        break;
      }

      continue;
    }

    if (isSeparator(ch)) {
      tokens.emplace_back(getSeparatorTokenType(ch), std::string(1, ch), line,
                          char_pos++);
      continue;
    }

    if (isIdentifierStart(ch)) {
      size_t start = i;
      while (i < input.size() && isIdentifierChar(input[i])) {
        i++;
      }
      std::string word = input.substr(start, i - start);
      i--; // Step back to process the current character again
      TokenType type =
          isKeyword(word) ? TokenType::Keyword : TokenType::Identifier;
      tokens.emplace_back(type, word, line, char_pos);
      char_pos += word.size();
      continue;
    }

    if (isNumberChar(ch)) {
      size_t start = i;
      bool is_float = false;

      while (i < input.size() && isNumberChar(input[i])) {
        i++;
        if (input[i] == '.' && !is_float) {
          is_float = true;
          i++; // skip the . so that the while loop continues
        } else if (input[i] == '.' && is_float) {
          throw std::invalid_argument("found float with two .");
        }
      }

      std::string number = input.substr(start, i - start);
      i--; // Step back to process the current character again

      if (!is_float) {
        tokens.emplace_back(TokenType::Integer, number, line, char_pos);
      } else {
        tokens.emplace_back(TokenType::Float, number, line, char_pos);
      }
      char_pos += number.size();

      continue;
    }

    if (ch == '"') { // Handle string literals

      size_t start = i;
      std::string str_value;
      // skip the current "
      i++;

      while (i < input.size() && input[i] != '"') {
        str_value += input[i];
        i++;
      }

      if (i < input.size() && input[i] == '"') {
        // dont need to skip the closing ", since it gets
        // skipped on the next loop iteration
        tokens.emplace_back(TokenType::String, str_value, line, char_pos);
        char_pos += (i - start);

      } else { // this triggers when we reach the end of the file
        // Unterminated string literal
        tokens.emplace_back(TokenType::Unknown, input.substr(start), line,
                            char_pos);
        break;
      }
      continue; // goto next loop to not add unknown token
    }

    // If character doesn't fit any category, add it as unknown token
    tokens.emplace_back(TokenType::Unknown, std::string(1, ch), line,
                        char_pos++);
  }

  return tokens;
}

void printTokens(const std::vector<Token> &tokens) {
  int i = 0;
  for (const auto &token : tokens) {
    std::cout << i << " Token(Type: " << static_cast<int>(token.type) << ", Value: \""
              << token.value << "\", Line: " << token.line
              << ", CharPos: " << token.char_pos << ")\n";
    i++;
  }
}

int test_lexer() {
  std::string input = R"(
    int main() {
      float a = 3;
    //this is a comment
      return 3.14;
    }

    $ here is 
    a multi
    line
    comment $ int hello = 3; 
    print$hdeiwhui$("hello world!---");
    )";

  Lexer lexer;
  std::vector<Token> tokens = lexer.tokenize(input);

  printTokens(tokens);
  return 0;
}
