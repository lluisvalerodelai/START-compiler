#include "lexerpp.hpp"
#include <cmath>
#include <cstdio>
#include <iostream>
#include <stack>
#include <string>
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
  char_type,
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
  virtual ~expression_node() = default;
};

class string_expression_node : public expression_node {
public:
  const std::string string_value;

  string_expression_node(std::string s) : string_value(s) {}

  virtual ~string_expression_node() = default;
};

class int_expression_node : public expression_node {
public:
  const int int_value;

  int_expression_node(int i) : int_value(i) {}

  virtual ~int_expression_node() = default;
};

class float_expression_node : public expression_node {
public:
  const float float_value;

  float_expression_node(float f) : float_value(f) {}

  virtual ~float_expression_node() = default;
};

class Lang {
public:
  std::vector<Token> tokens;
  int line_nr = 0;
  bool buffer_occupied = false;
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
    if (!parse_dec_ass(parent_node) && !parse_print_statement(parent_node)) {
      return false;
    }
    if (!parse_semicolon()) {
      return false;
    }
    clear_stack(); // need to clear stack again after checking for semicolon
                   // since we dont take care of it in previous functs
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

  bool parse_print_statement(base_ast_node *parent_node) {

    base_ast_node *possible_child_node = new base_ast_node;
    possible_child_node->node_type = AST_node_type::print;

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.value != "print") {
      stack_collapse();
      return false;
    }

    token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.value != "(") {
      stack_collapse();
      return false;
    }

    if (!parse_expression(possible_child_node)) {
      return false;
    }

    token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.value != ")") {
      stack_collapse();
      return false;
    }

    // if we have succesfully parsed a print statement, then we link it with its
    // parent
    parent_node->children.emplace_back(possible_child_node);
    possible_child_node->parent = parent_node;
    clear_stack();
    return true;
  }

  // important distinction to be made here
  // parse_type_identifier does not parse for an identifier, it parses for the
  // keyword that identifies some type
  bool parse_type_identifier(base_ast_node *parent) {
    type_identifier_node *possible_child_node = new type_identifier_node;
    possible_child_node->node_type = AST_node_type::type_identifier;

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.value != "int" && token_next.value != "string" &&
        token_next.value != "float") {
      return false;
    } else {
      if (token_next.value == "int") {
        possible_child_node->type = variable_types::int_type;
      }
      if (token_next.value == "float") {
        possible_child_node->type = variable_types::float_type;
      }
      if (token_next.value == "string") {
        possible_child_node->type = variable_types::string_type;
      }
    }

    possible_child_node->parent = parent;
    parent->children.emplace_back(possible_child_node);
    return true;
  }

  bool parse_identifier(base_ast_node *parent) {
    identifier_node *potential_child = new identifier_node;
    potential_child->node_type = AST_node_type::identifier_name;

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.type != token_type::identifier) { // if not parse identifier
      stack_collapse();
      return false;
    } else {
      potential_child->value = token_next.value;
    }

    potential_child->parent = parent;
    parent->children.emplace_back(potential_child);
    return true;
  }

  bool parse_dec_ass(base_ast_node *parent) {
    base_ast_node *possible_child_node = new base_ast_node;
    possible_child_node->node_type = AST_node_type::declare_and_assign;

    if (!parse_type_identifier(possible_child_node)) {
      stack_collapse();
      return false;
    }

    if (!parse_identifier(possible_child_node)) { // if not parse identifier
      stack_collapse();
      return false;
    }

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.value != "=") {
      stack_collapse();
      return false;
    }

    if (!parse_expression(possible_child_node)) { // if not parse literal
      stack_collapse();
      return false;
    }

    possible_child_node->parent = parent;
    parent->children.emplace_back(possible_child_node);
    clear_stack();
    return true;
  }

  bool parse_expression(base_ast_node *parent) {
    expression_node *potential_child = new expression_node;
    potential_child->node_type = AST_node_type::expression;

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.type != token_type::literal) {
      stack_collapse();
      return false;
    } else {
      // TODO: this : make sure to name whichever token you create
      // potential_child
      //  if its an int
      //    check that its the same type as the type it was declared as
      //    if yes
      //      make a int node type
      //  if its a float
      //    check that its the same type as the type it was declared as
      //    if yes
      //      make a float node type
      //  if its a string
      //    check that its the same type as the type it was declared as
      //    if yes
      //      make a float node type

      //
    }

    potential_child->parent = parent;
    parent->children.emplace_back(potential_child);
    return true;
  }
};

// return number of tokens generated by lexer()
int count_tokens(Token *tokens_list) {
  int count = 0;

  while (tokens_list[count].type != END) {
    count++;
  }

  return count;
}

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

  if (const identifier_node *i_node = dynamic_cast<identifier_node *>(node)) {
    std::cout << i_node->value;
  }

  if (const type_identifier_node *i_node =
          dynamic_cast<type_identifier_node *>(node)) {
    if (i_node->type == variable_types::int_type) {
      std::cout << "int";
    } else if (i_node->type == variable_types::char_type) {
      std::cout << "char";
    } else if (i_node->type == variable_types::float_type) {
      std::cout << "float";
    } else if (i_node->type == variable_types::char_type) {
      std::cout << "char";
    } else if (i_node->type == variable_types::string_type) {
      std::cout << "string";
    }
  }

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
