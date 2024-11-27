#include "lexer.hpp"

/*
 *
 * Defined tokens:
 *  -separators: ( ) { } ; " "
 *  -operators: + - * / ! || &&
 *  -keywords: int, float, char, return, print, if, else, while, for
 *  -identifier specs: can contain '_' but no '-' also can not start with
 * numbers
 */

std::vector<Token> Lexer::lex(const std::string &raw_input) {

  const std::string string_to_process = raw_input;
  std::vector<Token> tokens;
  int global_char = 0;
  int local_line_char = 0;
  int line_nr;

  /*
   * while not at end token
   *  if is_single_char
   *    add token
   *  if is numeric
   *    add token
   *  if is identifier
   *    add token
   * */

  while (!raw_input.empty()) {
    char current_char = raw_input[global_char];

    if (current_char == ' ') {
      std::cout << "found a space \n";
      break;
    }

    switch (current_char) {
    case '\r':
      std::cout << "found a registered at line " << line_nr << " character "
                << local_line_char << "\n";
      line_nr += 1;
      local_line_char = 0;
    case '\n':
      std::cout << "found a registered at line " << line_nr << " character "
                << local_line_char << "\n";
      line_nr += 1;
      local_line_char = 0;
    }

    global_char += 1;
    local_line_char += 1;
  }

  return tokens;
}

int main(int argc, char **argv) {

  std::string example_string = " ";

  Lexer lex;
  auto tokens = lex.lex(example_string);

  std::cout << "Type: " << tokens[0].type << "Content: " << tokens[0].content;

  return 0;
}
