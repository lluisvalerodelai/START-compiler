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
    std::cout << tokens_i.value << "\t";
    std::cout << tokens_i.characterPosition << std::endl;
  }
}

std::vector<Token> lexer(const std::string& file_name) {
    std::ifstream F{file_name, std::ifstream::in | std::ifstream::binary};
    // If we can not open the file
    if (F.fail()) {
      std::cerr << "[C++] Error: " << strerror(errno) << std::endl;
      return std::vector<Token>{};
    }

    return lexer(F);
}

template <class Elem, class Traits>
std::vector<Token> lexer(std::basic_istream<Elem, Traits>& F) {
  char c;
  bool are_checking_string = false;

  std::vector<Token> token_list;
  token_list.reserve(100);

  // main loop
  while (c = F.get(), c >= 0) {
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
      token_list.emplace_back(token_type::separator, "{", static_cast<int>(F.tellg()) - 1);
    } else if (c == '}') {
      token_list.emplace_back(token_type::separator, "}", static_cast<int>(F.tellg()) - 1);
    } else if (c == '(') {
      token_list.emplace_back(token_type::separator, "(", static_cast<int>(F.tellg()) - 1);
    } else if (c == ')') {
      token_list.emplace_back(token_type::separator, ")", static_cast<int>(F.tellg()) - 1);
    } else if (c == '[') {
      token_list.emplace_back(token_type::separator, "[", static_cast<int>(F.tellg()) - 1);
    } else if (c == ']') {
      token_list.emplace_back(token_type::separator, "]", static_cast<int>(F.tellg()) - 1);
    } else if (c == ';') {
      token_list.emplace_back(token_type::separator, ";", static_cast<int>(F.tellg()) - 1);
    } else if (c == '=') {

      // check if next char is a =, in which case the token is ==

      char next_char = F.peek();

      if (next_char == '=') {
        token_list.emplace_back(token_type::operator_token,
                                "==", static_cast<int>(F.tellg()) - 1);

        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        token_list.emplace_back(token_type::operator_token, "=",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if (c == '+') {

      char next_char = F.peek();

      if (next_char == '=') {
        token_list.emplace_back(token_type::operator_token,
                                "+=", static_cast<int>(F.tellg()) - 1);

        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        token_list.emplace_back(token_type::operator_token, "+",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if (c == '-') {
      char next_char = F.peek();

      if (next_char == '=') {
        token_list.emplace_back(token_type::operator_token,
                                "-=", static_cast<int>(F.tellg()) - 1);

        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        token_list.emplace_back(token_type::operator_token, "-",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if (c == '*') {
      char next_char = F.peek();

      if (next_char == '=') {
        token_list.emplace_back(token_type::operator_token,
                                "*=", static_cast<int>(F.tellg()) - 1);

        // move ahead by 2 to get to the next whitespace so we dont read the
        // next = on the next cycle
        c = F.get();
        c = F.get();
      } else {
        token_list.emplace_back(token_type::operator_token, "*",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if (c == '/') {
      // check if the next one isnt /, in which case add a comment token and go
      // to start of next line
      char next_c = F.get();
      if (next_c == '/') // One line comment
      {
        // Reads the rest of the line and advances the file pointer to the next
        // line's first char
        std::string commentBuffer;
        std::getline(F, commentBuffer);

        token_list.emplace_back(token_type::comment, std::string("//" + commentBuffer),
                                static_cast<int>(F.tellg()) - 2);
      } else if (next_c == '*') // Multiline comment
      {
        std::string commentBuffer("/*");

        while (true) {
          c = F.get();

          // Look for ending
          if (c == '*') {
            next_c = F.peek();
            
            if (next_c == '/') {
              F.get();
              commentBuffer += "*/";
              break;
            }
          }

          commentBuffer += c;
        }

        token_list.emplace_back(
            token_type::comment,
            commentBuffer,
            static_cast<int>(F.tellg()) - 1);
      } else {
        // if its none of those, add a / token
        token_list.emplace_back(token_type::operator_token, "/",
                                static_cast<int>(F.tellg()) - 1);
      }
      // check if its a number,
    } else if (c == '<') {
      char next_c = F.peek();
      if (next_c == '=') {
        c = F.get();
        token_list.emplace_back(token_type::operator_token,
                                "<=", static_cast<int>(F.tellg()) - 1);
      } else {
        token_list.emplace_back(token_type::operator_token, "<",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if (c == '>') {
      char next_c = F.peek();
      if (next_c == '=') {
        c = F.get();
        token_list.emplace_back(token_type::operator_token,
                                ">=", static_cast<int>(F.tellg()) - 1);
      } else {
        token_list.emplace_back(token_type::operator_token, ">",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if (c == '!') {
      char next_c = F.peek();
      if (next_c == '=') {
        c = F.get();
        token_list.emplace_back(token_type::operator_token,
                                "!=", static_cast<int>(F.tellg()) - 1);
      } else {
        token_list.emplace_back(token_type::operator_token, "!",
                                static_cast<int>(F.tellg()) - 1);
      }
    } else if ((isdigit(c) != 0 || c == '.')) // TODO: This is not doing what we
                                              // want it to do, fix later
    {
      bool used_point = false;
      std::string number;
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

      token_list.emplace_back(token_type::literal, number,
                              static_cast<int>(F.tellg()) - 1); // Number literal
    } else if (c == '"') {
      std::string val;
      val += '"';
      c = F.get();
      while (c != '"') {
        val += c;
        c = F.get();
      }
      val += '"';
      token_list.emplace_back(token_type::literal, val,
                              static_cast<int>(F.tellg()) - 1); // String literal
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
        token_list.emplace_back(token_type::keyword, buffer,
                                static_cast<int>(F.tellg()) - 1); // Built in keyword
      } else {
        token_list.emplace_back(
            token_type::identifier, buffer,
            static_cast<int>(F.tellg()) - 1); // User defined variable / type
      }
    }
  }

  token_list.emplace_back(
      token_type::END, "END",
      static_cast<int>(F.tellg()) - 1); // Add end token to indicate end of file

  return token_list;
}

int lexer_tester(int argc, char **argv) {

  string file_name = argv[1];

  std::vector<Token> tokens = lexer(file_name);
  pretty_print_tokens(tokens);
  return 0;
}
