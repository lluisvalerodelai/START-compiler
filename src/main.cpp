#include "codegen.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include <fstream>
#include <iostream>

void printAST(ASTNode *node, int depth = 0) {
  if (!node)
    return;
  std::cerr << std::string(depth * 2, ' ') << node->value << "\n";
  for (ASTNode *child : node->children) {
    printAST(child, depth + 1);
  }
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <source_file>\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Error: cannot open file " << argv[1] << "\n";
    return 1;
  }

  std::string source((std::istreambuf_iterator<char>(file)),
                     std::istreambuf_iterator<char>());
  file.close();

  Lexer lexer;
  std::vector<Token> tokens = lexer.tokenize(source);

  Parser parser(tokens);
  ASTNode *ast = parser.parse();

  CodeGenerator codegen;
  std::string assembly = codegen.generate(ast);

  std::cout << assembly;

  delete ast;
  return 0;
}
