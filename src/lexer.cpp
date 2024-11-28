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
  int line_nr; //consider just having a position struct?

  /*
   * while not at end token (or string is not empty)
   *  if is one of the single character tokens
   *    add corresponding token
   *    the only thing to be careful with is the '/' token, if the character after a '/' is either a '/' or '*' then its a comment, so handle it there
   *  if is a number
   *    add token (change the type of the token struct to also hold an int for if its an int or a float for if its a float, not just a string)
   *  else
   *  	its either an identifier, or keyword
   *  	check with known keywords
   *  	else add identifier
   *	
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
