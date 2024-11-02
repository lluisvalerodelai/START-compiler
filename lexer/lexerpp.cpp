#include "lexerpp.hpp"

using std::string;

void pretty_print_tokens(std::vector<Token> tokens) {
  for (int i = 0; i < tokens.size(); i++) {
    std::cout << "Token " << i << ": " << tokens[i].type << "\t"
              << tokens[i].value << std::endl;
  }
}

std::vector<Token> lexer(std::ifstream &F) {
  char c;
  bool are_checking_string = false;

  std::vector<Token> token_list;
  token_list.reserve(100);
  // main loop

  while (!F.eof()) {
    c = F.get();
    std::cout << "current char: " << c << std::endl;

    // check if its a skippable char
    if (!are_checking_string) {
      switch (c) {
      case '\b':
        continue;
      case '\f':
        continue;
      case '\n':
        continue;
      case ' ':
        continue;
      case '\t':
        continue;
      case '\e':
        continue;
      }
    }

    // check if its a separator

    if (c == '{') {
      Token separator_token{.type = token_type::separator, .value = };
      token_list.emplace_back();
    } else if (c == '}') {

    } else if (c == '(') {

    } else if (c == ')') {

    } else if (c == '[') {

    } else if (c == ']') {

    } else if (c == ';') {

    } else if (c == '=') {
      // check if the next char is also an ==, in which case we add that instead
      // of a =
    } else if (c == '+') {

    } else if (c == '-') {

    } else if (c == '*') {

    } else if (c == '/') {
      // check if the next one is also a /, in which case add a comment token in
      // which case you go to start of next line check if the next one is also a
      // *, in which case you keep moving forwards till you find a */
    }

    // check if its a number,
    // for as long as the char were on is a number, add it to the buffer, unless
    // its a . if its a . we check if we have already added a . before and if
    // not then we add it

    // check for a string, when we see a ", fill up the buffer until the next ".
    // turn off checks for ignorable characters
    //
    //  if its none of the above, then its either a keyword or an identifier, in
    //  which case we fill up the buffer for as long as were in an alphanum
    //  character until we see skippable chars
  }

  Token end_token{.type = token_type::END};
  token_list.emplace_back(end_token);
  return token_list;
}

int main(int argc, char **argv) {

  string file_name = argv[1];

  std::ifstream fileObject{file_name};
  lexer(fileObject);

  return 0;
}
