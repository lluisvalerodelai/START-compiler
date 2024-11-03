#include "lexerpp.hpp"

using std::string;

void pretty_print_tokens(std::vector<Token> tokens) {
  for (int i = 0; i < tokens.size(); i++) {
    Token tokens_i = tokens[i];
    std::string type_str;
    if (tokens_i.type == token_type::identifier) {
      type_str = "identifier";
    } else if (tokens_i.type == token_type::keyword) {
      type_str = "keyword";
    } else if (tokens_i.type == token_type::separator) {
      type_str = "separator";
    } else if (tokens_i.type == token_type::operator_token) {
      type_str = "operator";
    } else if (tokens_i.type == token_type::literal) {
      type_str = "literal";
    } else if (tokens_i.type == token_type::END) {
      type_str = "END";
    }
    std::cout << type_str << "\t";
    std::cout << tokens_i.value << std::endl;
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
      Token separator_token{.type = token_type::separator, .value = "{"};
      token_list.emplace_back(separator_token);
    } else if (c == '}') {
      Token separator_token{.type = token_type::separator, .value = "}"};
      token_list.emplace_back(separator_token);
    } else if (c == '(') {
      Token separator_token{.type = token_type::separator, .value = "("};
      token_list.emplace_back(separator_token);
    } else if (c == ')') {
      Token separator_token{.type = token_type::separator, .value = ")"};

      token_list.emplace_back(separator_token);
    } else if (c == '[') {
      Token separator_token{.type = token_type::separator, .value = "["};

      token_list.emplace_back(separator_token);
    } else if (c == ']') {
      Token separator_token{.type = token_type::separator, .value = "]"};

      token_list.emplace_back(separator_token);
    } else if (c == ';') {
      Token separator_token{.type = token_type::separator, .value = ";"};
      token_list.emplace_back(separator_token);
    } else if (c == '=') {

      // check if next char is a =, in which case the token is ==

      char next_char = F.peek();

      if (next_char == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = "=="};
        token_list.emplace_back(separator_token);
        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = "="};
        token_list.emplace_back(separator_token);
      }

    } else if (c == '+') {

      char next_char = F.peek();

      if (next_char == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = "+="};
        token_list.emplace_back(separator_token);
        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = "+"};
        token_list.emplace_back(separator_token);
      }

    } else if (c == '-') {
      char next_char = F.peek();

      if (next_char == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = "-="};
        token_list.emplace_back(separator_token);
        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = "-"};
        token_list.emplace_back(separator_token);
      }

    } else if (c == '*') {
      char next_char = F.peek();

      if (next_char == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = "*="};
        token_list.emplace_back(separator_token);
        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = "*"};
        token_list.emplace_back(separator_token);
      }
    } else if (c == '/') {
      // check if the next one isnt /, in which case add a comment token and go
      // to start of next line
      char next_c = F.get();
      if (next_c == '/') {
        // consume the next /
        c = F.get();
        // consume chars until we hit a line break char
        while (c != '\n') {
          c = F.get();
        }
        // first consume the last eol char, and check for \r\n eols
        if (c == '\n') {
          c = F.get();
        }
      } else if (next_c == '*') {
        // consume next char
        c = F.get();
        c = F.get();
        while (c != '*' && !F.eof()) {
          c = F.get();
        }
        // we are at C = *, move to next char which will be /
        c = F.get();
      } else {
        // if its none of those, add a / token
        Token separator_token{.type = token_type::operator_token, .value = "/"};
        token_list.emplace_back(separator_token);
      }
      // check if its a number,
    } else if (c == '<') {
      char next_c = F.peek();
      if (next_c == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = "<="};
        token_list.emplace_back(separator_token);
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = "<"};
        token_list.emplace_back(separator_token);
      }

    } else if (c == '>') {
      char next_c = F.peek();
      if (next_c == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = ">="};
        token_list.emplace_back(separator_token);
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = ">"};
        token_list.emplace_back(separator_token);
      }
    } else if (c == '!') {
      char next_c = F.peek();
      if (next_c == '=') {
        Token separator_token{.type = token_type::operator_token,
                              .value = "!="};
        token_list.emplace_back(separator_token);
        c = F.get();
      } else {
        Token separator_token{.type = token_type::operator_token, .value = "!"};
        token_list.emplace_back(separator_token);
      }
    } else if ((isdigit(c) != 0 || c == '.')) {
      bool used_point = false;
      std::string number = "";
      number += c;
      if (c == '.') {
        used_point = true;
      }
      // is the current char a number?, then add it to the list and move on to
      // the next and repeat
      char next_c = F.peek();
      while (isdigit(next_c) || next_c == '.') {
        if (next_c == '.' && used_point) {
          break;
        }
        c = F.get();
        number += c;
        next_c = F.peek();
        if (!isdigit(F.peek()) && next_c != '.') {
          break;
        }
      }

      Token numeric{.type = token_type::literal, .value = number};
      token_list.emplace_back(numeric);

    } else if (c == '"') {
      std::string val = "";
      c = F.get();
      while (c != '"') {
        val += c;
        c = F.get();
      }
      Token str{.type = token_type::literal, .value = val};
      token_list.emplace_back(str);
    } else {
      std::string buffer = "";
      while (isalnum(c)) {
        buffer += c;
        char next_c = F.peek();
        if (isalnum(next_c)) {
          c = F.get();
        } else {
          break;
        }
      }
      // check against all the known keywords
      if (buffer == "int" || buffer == "float" || buffer == "char" ||
          buffer == "bool" || buffer == "string" || buffer == "return" ||
          buffer == "for" || buffer == "while" || buffer == "if" ||
          buffer == "else" || buffer == "elif" || buffer == "function" ||
          buffer == "returns" || buffer == "print") {
        Token buf{.type = token_type::keyword, .value = buffer};
        token_list.emplace_back(buf);
      } else {
        Token buf{.type = token_type::identifier, .value = buffer};
        token_list.emplace_back(buf);
      }
    }
  }
  Token end_token{.type = token_type::END, .value = "END"};
  token_list.emplace_back(end_token);
  return token_list;
}

int main(int argc, char **argv) {

  string file_name = argv[1];

  std::ifstream fileObject{file_name};

  std::vector<Token> tokens = lexer(fileObject);
  pretty_print_tokens(tokens);
  return 0;
}
