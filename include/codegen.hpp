#pragma once
#include "parser.hpp"
#include <map>
#include <sstream>

class CodeGenerator {
private:
  std::ostringstream asm_code;
  std::map<std::string, int> var_offsets;
  int stack_offset = 0;
  int label_counter = 0;

  std::string new_label();
  void gen_expression(ASTNode *node);
  void gen_statement(ASTNode *node);
  void gen_statements(ASTNode *node);
  void gen_declaration(ASTNode *node);
  void gen_assignment(ASTNode *node);
  void gen_if(ASTNode *node);
  void gen_while(ASTNode *node);

public:
  std::string generate(ASTNode *root);
};
