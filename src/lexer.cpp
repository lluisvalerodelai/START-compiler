#include "lexer.hpp"

/*
 *
 * Defined tokens:
 *  -separators: ( ) { } ; " "
 *  -operators: + - * / ! || &&
 *  -keywords: int, float, char, return, print, if, else, while, for
 *  -identifier specs: can contain '_' but no '-' also can not start with numbers
 */

std::vector<Token> Lexer::lex(const std::string& raw_input) {

    const std::string& string_to_process = raw_input;
    std::vector<Token> tokens;
    
    // Do lexer stuff...

    tokens.emplace_back(Token::Position(), token_type::quotes, "TEST_CONTENT");
    processed_string += string_to_process;

    return tokens;
}

int main(int argc, char **argv) {

  std::string example_string = "hello world";

  Lexer lex;
  auto tokens = lex.lex(example_string);

  std::cout <<  "Type: " << tokens[0].type << "Content: " << tokens[0].content;

  return 0;
}
