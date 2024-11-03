#include "lexerpp.hpp"

using std::string;

void pretty_print_tokens(std::vector<Token> tokens)
{
  for (int i = 0; i < tokens.size(); i++)
  {
    Token tokens_i = tokens[i];
    std::string type_str;
    if (tokens_i.type == token_type::identifier)
    {
      type_str = "identifier";
    }
    else if (tokens_i.type == token_type::keyword)
    {
      type_str = "keyword";
    }
    else if (tokens_i.type == token_type::separator)
    {
      type_str = "separator";
    }
    else if (tokens_i.type == token_type::operator_token)
    {
      type_str = "operator";
    }
    else if (tokens_i.type == token_type::literal)
    {
      type_str = "literal";
    }
    else if (tokens_i.type == token_type::END)
    {
      type_str = "END";
    }
    std::cout << type_str << "\t";
    std::cout << tokens_i.value << "\t";
    std::cout << tokens_i.characterPosition << std::endl;
  }
}

std::vector<Token> lexer(std::ifstream &F)
{
  char c;
  bool are_checking_string = false;

  std::vector<Token> token_list;
  token_list.reserve(100);
  // main loop

  while (!F.eof())
  {
    c = F.get();

    // check if its a skippable char
    if (!are_checking_string)
    {
      switch (c)
      {
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

    if (c == '{')
    {
      token_list.emplace_back(token_type::separator, "{", token_list.size());
    }
    else if (c == '}')
    {
      token_list.emplace_back(token_type::separator, "}", token_list.size());
    }
    else if (c == '(')
    {
      token_list.emplace_back(token_type::separator, "(", token_list.size());
    }
    else if (c == ')')
    {
      token_list.emplace_back(token_type::separator, ")", token_list.size());
    }
    else if (c == '[')
    {
      token_list.emplace_back(token_type::separator, "[", token_list.size());
    }
    else if (c == ']')
    {
      token_list.emplace_back(token_type::separator, "]", token_list.size());
    }
    else if (c == ';')
    {
      token_list.emplace_back(token_type::separator, ";", token_list.size());
    }
    else if (c == '=')
    {

      // check if next char is a =, in which case the token is ==

      char next_char = F.peek();

      if (next_char == '=')
      {
        token_list.emplace_back(token_type::operator_token, "==", token_list.size());

        // move ahead by 2 to get to the next whitespace so we dont read the next = on the next cycle
        c = F.get();
        c = F.get();
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, "=", token_list.size());
      }
    }
    else if (c == '+')
    {

      char next_char = F.peek();

      if (next_char == '=')
      {
        token_list.emplace_back(token_type::operator_token, "+=", token_list.size());

        // move ahead by 2 to get to the next whitespace so we dont read the next = on the next cycle
        c = F.get();
        c = F.get();
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, "+", token_list.size());
      }
    }
    else if (c == '-')
    {
      char next_char = F.peek();

      if (next_char == '=')
      {
        token_list.emplace_back(token_type::operator_token, "-=", token_list.size());

        // move ahead by 2 to get to the next whitespace so we dont read the next = on the next cycle
        c = F.get();
        c = F.get();
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, "-", token_list.size());
      }
    }
    else if (c == '*')
    {
      char next_char = F.peek();

      if (next_char == '=')
      {
        token_list.emplace_back(token_type::operator_token, "*=", token_list.size());

        // move ahead by 2 to get to the next whitespace so we dont read the next = on the next cycle
        c = F.get();
        c = F.get();
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, "*", token_list.size());
      }
    }
    else if (c == '/')
    {
      // check if the next one isnt /, in which case add a comment token and go to start of next line
      char next_c = F.get();
      if (next_c == '/') // One line comment
      {
        // Reads the rest of the line and advances the file pointer to the next line's first char
        std::string commentBuffer;
        std::getline(F, commentBuffer);
        token_list.emplace_back(token_type::comment, commentBuffer, token_list.size());
      }
      else if (next_c == '*') // Multiline comment
      {
        std::ostringstream multilineCommentBuffer;
        std::string commentBuffer;

        do
        {
          std::getline(F, commentBuffer);
          multilineCommentBuffer << commentBuffer << "\n";
        
        // The multiline comment stops once we hit a */ NOTE: We are expecting that no character will follow after */ in the same line!
        } while (commentBuffer.size() >= 2 && commentBuffer[commentBuffer.size() - 2] == '*' && commentBuffer.back() == '/');

        token_list.emplace_back(token_type::comment, multilineCommentBuffer.str(), token_list.size());
      }
      else
      {
        // if its none of those, add a / token
        token_list.emplace_back(token_type::operator_token, "/", token_list.size());
      }
      // check if its a number, 
    }
    else if (c == '<')
    {
      char next_c = F.peek();
      if (next_c == '=')
      {
        c = F.get();
        token_list.emplace_back(token_type::operator_token, "<=", token_list.size());
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, "<", token_list.size());
      }
    }
    else if (c == '>')
    {
      char next_c = F.peek();
      if (next_c == '=')
      {
        c = F.get();
        token_list.emplace_back(token_type::operator_token, ">=", token_list.size());
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, ">", token_list.size());
      }
    }
    else if (c == '!')
    {
      char next_c = F.peek();
      if (next_c == '=')
      {
        c = F.get();
        token_list.emplace_back(token_type::operator_token, "!=", token_list.size());
      }
      else
      {
        token_list.emplace_back(token_type::operator_token, "!", token_list.size());
      }
    }
    else if ((isdigit(c) != 0 || c == '.')) // TODO: This is not doing what we want it to do, fix later
    {
      bool used_point = false;
      std::ostringstream number;
      number << c;
      if (c == '.')
      {
        used_point = true;
      }
      // is the current char a number?, then add it to the list and move on to the next and repeat
      char next_c = F.peek();
      while (isdigit(next_c) || next_c == '.')
      {
        if (next_c == '.' && used_point)
        {
          break;
        }
        c = F.get();
        number << c;
        next_c = F.peek();
        if (!isdigit(F.peek()) && next_c != '.')
        {
          break;
        }
      }

      token_list.emplace_back(token_type::literal, number.str(), token_list.size()); // Number literal
    }
    else if (c == '"')
    {
      std::ostringstream val;
      c = F.get();
      while (c != '"')
      {
        val << c;
        c = F.get();
      }
      token_list.emplace_back(token_type::literal, val.str(), token_list.size()); // String literal
    }
    else
    {
      std::string buffer = "";
      while (isalnum(c))
      {
        buffer += c;
        char next_c = F.peek();
        if (isalnum(next_c))
        {
          c = F.get();
        }
        else
        {
          break;
        }
      }
      // check against all the known keywords
      if (buffer == "int" || buffer == "float" ||
          buffer == "char" || buffer == "bool" ||
          buffer == "string" || buffer == "return" ||
          buffer == "for" || buffer == "while" ||
          buffer == "if" || buffer == "else" ||
          buffer == "elif" || buffer == "function" ||
          buffer == "returns" || buffer == "print")
      {
        token_list.emplace_back(token_type::keyword, buffer, token_list.size()); // Built in keyword
      }
      else
      {
        token_list.emplace_back(token_type::identifier, buffer, token_list.size()); // User defined variable / type
      }
    }
  }

  token_list.emplace_back(token_type::END, "END", token_list.size()); // Add end token to indicate end of file
  
  return token_list;
}

int main(int argc, char **argv)
{

  string file_name = argv[1];

  std::ifstream fileObject{file_name};

  std::vector<Token> tokens = lexer(fileObject);
  pretty_print_tokens(tokens);
  return 0;
}
