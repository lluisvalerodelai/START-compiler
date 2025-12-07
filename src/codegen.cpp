#include "codegen.hpp"
#include <iostream>

std::string CodeGenerator::new_label() {
  return ".L" + std::to_string(label_counter++);
}


// for expressions you generate the lSH, then push value, then RHS, then evaluate
void CodeGenerator::gen_expression(ASTNode *node) {

  if (node->value == "+") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  add %rbx, %rax\n";

  } else if (node->value == "-") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  sub %rbx, %rax\n";

  } else if (node->value == "*") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  imul %rbx, %rax\n";

  } else if (node->value == "/") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cqo\n";
    asm_code << "  idiv %rbx\n";

  } else if (node->value == "<") {
    
    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cmp %rbx, %rax\n";
    asm_code << "  setl %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (node->value == ">") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cmp %rbx, %rax\n";
    asm_code << "  setg %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (node->value == "<=") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cmp %rbx, %rax\n";
    asm_code << "  setle %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (node->value == ">=") {
    
    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cmp %rbx, %rax\n";
    asm_code << "  setge %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (node->value == "==") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cmp %rbx, %rax\n";
    asm_code << "  sete %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (node->value == "!=") {

    gen_expression(node->children[1]);
    asm_code << "  push %rax\n";
    gen_expression(node->children[0]);
    asm_code << "  pop %rbx\n";
    asm_code << "  cmp %rbx, %rax\n";
    asm_code << "  setne %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (node->value == "&") {

    gen_expression(node->children[0]);
    asm_code << "  cmp $0, %rax\n";
    std::string skip_label = new_label();
    asm_code << "  je " << skip_label << "\n";
    gen_expression(node->children[1]);
    asm_code << "  cmp $0, %rax\n";
    asm_code << "  setne %al\n";
    asm_code << "  movzbl %al, %eax\n";
    asm_code << skip_label << ":\n";

  } else if (node->value == "|") {

    gen_expression(node->children[0]);
    asm_code << "  cmp $0, %rax\n";
    std::string skip_label = new_label();
    asm_code << "  jne " << skip_label << "\n";
    gen_expression(node->children[1]);
    asm_code << "  cmp $0, %rax\n";
    asm_code << "  setne %al\n";
    asm_code << "  movzbl %al, %eax\n";
    asm_code << skip_label << ":\n";

  } else if (node->value == "!") {

    gen_expression(node->children[0]);
    asm_code << "  cmp $0, %rax\n";
    asm_code << "  sete %al\n";
    asm_code << "  movzbl %al, %eax\n";

  } else if (std::isdigit(node->value[0]) ||
             (node->value[0] == '-' && node->value.size() > 1)) {
    asm_code << "  mov $" << node->value << ", %rax\n";
  } else {
    if (var_offsets.find(node->value) != var_offsets.end()) {
      asm_code << "  mov " << var_offsets[node->value] << "(%rbp), %rax\n";
    } else {
      std::cerr << "Error: undefined variable " << node->value << "\n";
      exit(1);
    }
  }
}

void CodeGenerator::gen_declaration(ASTNode *node) {

  std::string var_name = node->children[0]->value;
  stack_offset -= 8;
  var_offsets[var_name] = stack_offset;

  gen_expression(node->children[1]);
  asm_code << "  mov %rax, " << stack_offset << "(%rbp)\n";
}

void CodeGenerator::gen_assignment(ASTNode *node) {
  std::string var_name = node->children[0]->value;
  gen_expression(node->children[1]);
  asm_code << "  mov %rax, " << var_offsets[var_name] << "(%rbp)\n";
}

void CodeGenerator::gen_if(ASTNode *node) {
  std::string end_label = new_label();

  gen_expression(node->children[0]);
  asm_code << "  cmp $0, %rax\n";
  asm_code << "  je " << end_label << "\n";

  gen_statements(node->children[1]);

  asm_code << end_label << ":\n";
}

void CodeGenerator::gen_while(ASTNode *node) {
  std::string start_label = new_label();
  std::string end_label = new_label();

  asm_code << start_label << ":\n";
  gen_expression(node->children[0]);
  asm_code << "  cmp $0, %rax\n";
  asm_code << "  je " << end_label << "\n";

  gen_statements(node->children[1]);

  asm_code << "  jmp " << start_label << "\n";
  asm_code << end_label << ":\n";
}

void CodeGenerator::gen_statement(ASTNode *node) {
  if (node->value == "declaration") {
    gen_declaration(node);
  } else if (node->value == "assignemnt node") {
    gen_assignment(node);
  } else if (node->value == "if node") {
    gen_if(node);
  } else if (node->value == "while node") {
    gen_while(node);
  }
}

void CodeGenerator::gen_statements(ASTNode *node) {
  for (ASTNode *child : node->children) {
    gen_statement(child);
  }
}

std::string CodeGenerator::generate(ASTNode *root) {
  asm_code << ".global main\n";
  asm_code << "main:\n";
  asm_code << "  push %rbp\n";
  asm_code << "  mov %rsp, %rbp\n";

  gen_statements(root);

  asm_code << "  mov $0, %rax\n";
  asm_code << "  mov %rbp, %rsp\n";
  asm_code << "  pop %rbp\n";
  asm_code << "  ret\n";

  return asm_code.str();
}
