#include "lexerpp.hpp"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

enum AST_node_type {
  program_node,
  print,
  declare_and_assign,
  expression,
  identifier_name,
  type_identifier,
};

enum variable_types {
  int_type,
  float_type,
  string_type,
};

class base_ast_node {
public:
  base_ast_node *parent = nullptr;
  std::vector<base_ast_node *> children;
  AST_node_type node_type; // temporary until hierarchy is added

  void set_parent(base_ast_node *parent_to_set) { parent = parent_to_set; }

  void add_child(base_ast_node *child_to_add) {
    children.emplace_back(child_to_add);
  }

  virtual ~base_ast_node() = default;
};

class identifier_node : public base_ast_node {
public:
  std::string value;

  void set_value(std::string identifier_name) { value = identifier_name; }

  virtual ~identifier_node() = default;
};

class type_identifier_node : public base_ast_node {
public:
  variable_types type;

  void set_value(variable_types t) { type = t; }

  virtual ~type_identifier_node() = default;
};

class expression_node : public base_ast_node {
public:
  std::variant<int, float, std::string> value;
  virtual ~expression_node() = default;
};

class Lang {
public:
  std::vector<Token> tokens;
  int line_nr = 0;
  std::stack<Token> token_wait_area;
  std::stack<Token> inline_tokens;
  std::vector<std::string> errors;

  Lang(std::vector<Token> toks) {
    toks.erase(toks.end() - 2);
    for (int i = toks.size() - 1; i >= 0; i--) {
      inline_tokens.push(toks[i]);
    }
  }

  void stack_collapse() {
    while (!token_wait_area.empty()) {
      Token top_token = token_wait_area.top();
      token_wait_area.pop();
      inline_tokens.push(top_token);
    }
  }

  void clear_stack() {
    while (!token_wait_area.empty()) {
      token_wait_area.pop();
    }
  }

  void add_error(std::string message) {
    errors.emplace_back("ERROR line " + std::to_string(line_nr) + ": " +
                        message);
  }

  Token next_token() {
    if (inline_tokens.empty()) {
      throw std::runtime_error("inline_tokens are empty");
    }

    Token top_token = inline_tokens.top();
    inline_tokens.pop();
    return top_token;
  };

  void print_inline_tokens() {
    std::cout << "wait area tokens: \n";

    while (!token_wait_area.empty()) {
      std::cout << token_wait_area.top().value << ", ";
      token_wait_area.pop();
    }

    std::cout << " \n inline tokens: \n";
    while (!inline_tokens.empty()) {
      std::cout << inline_tokens.top().value << ", ";
      inline_tokens.pop();
    }

    std::cout << std::endl;
  }

  bool parse_program(base_ast_node *program_node) {

    // program = <statement> + [statement + ...]
    program_node->node_type = AST_node_type::program_node;

    if (!parse_statement(program_node)) {
      return false;
    }

    while (inline_tokens.top().value != "END") {
      if (!parse_statement(program_node)) {
        return false;
      }
    }

    return true;
  }

  bool parse_statement(base_ast_node *parent_node) {

    // statement =  [<print_statement> || <variable_declare_assign>] + ";"
    // we dont need statement nodes as we have many other statement types of
    // node

    if (!parse_dec_ass(parent_node) && !parse_print_statement(parent_node)) {
      return false;
    }

    // check for if the case is {statement}

    if (!parse_semicolon()) {
      return false;
    }

    return true;
  }

  bool parse_semicolon() {
    Token token_next = next_token();
    token_wait_area.push(token_next);
    if (token_next.value != ";") {
      stack_collapse();
      return false;
    }
    return true;
  }

  bool parse_print_statement(base_ast_node *parent) {
    // fnheiuw
    return true;
  }

  bool parse_dec_ass(base_ast_node *parent) {
    // hfuriew
    return true;
  }
};

std::string give_node_type_string(base_ast_node *node) {

  switch (node->node_type) {
  case AST_node_type::program_node:
    return "program_node";
  case AST_node_type::print:
    return "print_node";
  case AST_node_type::identifier_name:
    return "Identifier_node";
  case AST_node_type::declare_and_assign:
    return "variable_declaration_&_assignment_node";
  case AST_node_type::expression:
    return "expression_node";
  case AST_node_type::type_identifier:
    return "type_identifier_node";
  }

  return "node was not assigned type";
}

void print_tree_from_node(base_ast_node *node, int depth) {
  std::cout << std::string(depth, '-') << give_node_type_string(node) << ": ";

  std::cout << give_node_type_string(node);

  std::cout << "\n";

  if (node->children.size() == 0) {
    return;
  } else {
    for (int i = 0; i < node->children.size(); i++) {
      print_tree_from_node(node->children[i], depth + 1);
    }
  }
}

void printGreen(const std::string text) {
  std::cout << "\033[32m" << text << "\033[0m" << std::endl;
}

void printRed(const std::string text) {
  std::cout << "\033[31m" << text << "\033[0m" << std::endl;
}

int main(int argc, char **argv) {

  std::string file_name = argv[1];

  std::ifstream fileObject{file_name};

  std::vector<Token> tokens = lexer(fileObject);

  Lang e(tokens);

  base_ast_node root;

  bool result = e.parse_program(&root);

  if (result) {
    printGreen("Compilation was a success!");
  } else {
    printRed("Compilation not succcesful :(");
  }

  print_tree_from_node(&root, 0);

  return 0;
}
