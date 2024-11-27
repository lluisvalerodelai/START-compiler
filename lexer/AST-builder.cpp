#include "lexerpp.hpp"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

enum variable_types {
  int_type,
  float_type,
  string_type,
};

enum bin_operators {
  plus,
  minus,
  times,
  divide,
};

class base_ast_node {
public:
  base_ast_node *parent = nullptr;
  std::vector<base_ast_node *> children;
  std::string node_identifier;

  void set_parent(base_ast_node *parent_to_set) { parent = parent_to_set; }

  void add_child(base_ast_node *child_to_add) {
    children.emplace_back(child_to_add);
  }

  virtual ~base_ast_node() = default;
};

class Lang {
public:
  std::vector<Token> tokens;
  int line_nr = 0;
  bool buffer_occupied = false;
  std::stack<Token> top_stack;
  std::stack<Token> bottom_stack;
  std::vector<std::string> errors;

  Lang(std::vector<Token> toks) {
    toks.erase(toks.end() - 2);
    for (int i = toks.size() - 1; i >= 0; i--) {
      bottom_stack.push(toks[i]);
    }
  }

  // move everything from the top stack onto the bottom stack
  void stack_collapse() {
    while (!top_stack.empty()) {
      Token top_token = top_stack.top();
      top_stack.pop();
      bottom_stack.push(top_token);
    }
  }

  // empty the top stack
  void clear_top_stack() {
    while (!top_stack.empty()) {
      top_stack.pop();
    }
  }

  void add_error(std::string message) {
    errors.emplace_back("ERROR line " + std::to_string(line_nr) + ": " +
                        message);
  }

  Token next_token() {
    if (bottom_stack.empty()) {
      throw std::runtime_error("inline_tokens are empty");
    }

    Token top_token = bottom_stack.top();
    bottom_stack.pop();
    return top_token;
  };

  bool parse_program(base_ast_node *program_node) {

    // program = <statement> + [statement + ...]

    program_node->node_identifier = "program_node";

    // try to parse the first statement
    if (!parse_statement(program_node)) {
      return false;
    }

    // keep parsing till we reach the end
    while (bottom_stack.top().value != "END") {
      if (!parse_statement(program_node)) {
        return false;
      }
    }

    return true;
  }

  bool parse_statement(base_ast_node *parent) {

    if (!parse_print_statement(parent) && !parse_var_dec_ass(parent)) {
      return false;
    }

    return true;
  }

  bool parse_print_statement(base_ast_node *parent) {

    // grab the first token
    Token top_token = next_token();
    top_stack.push(top_token);

    // check if its 'print'
    // if its not, go back to base setup (stack collapse)
    std::cout << top_token.value;
    if (top_token.value != "print") {
      stack_collapse();
      return false;
    }

    // get the next token
    top_token = next_token();

    if (top_token.value != "(") {
      stack_collapse();
      return false;
    }

    return true;
  }
  bool parse_var_dec_ass(base_ast_node *parent) { return true; }
};

void print_tree_from_node(base_ast_node *root, int depth) {
  // get the indentation right
  std::cout << std::string(0, ' ') << root->node_identifier << ": ";
  std::cout << std::endl;

  // print the children subtrees
  if (!root->children.empty()) {

    for (int i = 0; i < root->children.size(); i++) {
      print_tree_from_node(root->children[i], depth + 1);
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

  base_ast_node root;

  Lang e(tokens);
  bool result = e.parse_program(&root);

  if (result) {
    printGreen("Compilation was a success!");
  } else {
    printRed("Compilation not succcesful :(");
  }

  print_tree_from_node(&root, 0);

  return 0;
}
