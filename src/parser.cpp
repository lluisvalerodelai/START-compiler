#include "../include/parser.hpp"
#include "../include/lexer.hpp"

bool Parser::ParseVarAssignment() {

  //<var_assign> ::= identifier + "=" + expression

  std::cout << m_tokenStack.top().value;
  return true;
}

int main() {

  std::string test_input_1 = R"(
    int main() {
      float a = 3;
    //this is a comment
      return 3.14;
    }

    string a = "hello";

    $ here is 
    a multi
    line
    comment $ int hello = 3; 
    print("hellO");
  )";

  std::string input = R"(int a = 3;
  float b = 3.14563;char c = "c";
  string hello_world = "hello-wor21eid3w09qďe.k'ow+ d")";
  Lexer lexer;
  std::vector<Token> tokens = lexer.tokenize(input);

  printTokens(tokens);

  Parser P(tokens);
  P.ParseVarAssignment();

  return 0;
}
