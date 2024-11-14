#include "lexerpp.hpp"
#include <cstdio>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

struct AST_node {
  std::string type;
  AST_node *parent = nullptr;
  std::vector<AST_node *> children;
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

  bool parse_program(AST_node *program_node) {
    program_node->type = "program_node";
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

  bool parse_statement(AST_node *parent_node) {
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

  bool parse_print_statement(AST_node *parent_node) {

    AST_node *possible_child_node = new AST_node;

    possible_child_node->type = "print_node";

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
  bool parse_type_identifier(AST_node *parent) {
    AST_node *possible_child_node = new AST_node;
    possible_child_node->type = "type_identifier";

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.value != "int" && token_next.value != "string" &&
        token_next.value != "float") {
      stack_collapse();
      return false;
    }

    possible_child_node->parent = parent;
    parent->children.emplace_back(possible_child_node);
    return true;
  }

  bool parse_identifier(AST_node *parent) {
    AST_node *potential_child = new AST_node;
    potential_child->type = "identifier";

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.type != token_type::identifier) { // if not parse identifier
      stack_collapse();
      return false;
    }

    potential_child->parent = parent;
    parent->children.emplace_back(potential_child);
    return true;
  }

  bool parse_dec_ass(AST_node *parent) {
    AST_node *possible_child_node = new AST_node;
    possible_child_node->type = "declaration & assignment node";

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

  bool parse_expression(AST_node *parent) {
    AST_node *potential_child = new AST_node;
    potential_child->type = "expression";

    Token token_next = next_token();
    token_wait_area.push(token_next);

    if (token_next.type != token_type::literal) {
      stack_collapse();
      return false;
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

void print_tree_from_node(AST_node *node, int depth) {
  std::cout << std::string(depth, '-') << "node: " << node->type << "\n";

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

  AST_node root;

  Lang e(tokens);
  bool result = e.parse_program(&root);

  if (result) {
    printGreen("Compilation was a success!");
  } else {
    printRed("Compilation not succcesful :(");
  }

  print_tree_from_node(&root, 0);

  /* for testing purposes
  AST_node root;
  AST_node child1;
  AST_node child2;
  AST_node child3;
  AST_node c31;
  AST_node c32;

  root.type = "root";
  child1.type = "child1";
  child2.type = "child2";
  child3.type = "child3";
  c31.type = "c31";
  c32.type = "c32";
  root.children.emplace_back(&child1);
  root.children.emplace_back(&child3);
  root.children.emplace_back(&child2);

  child1.parent = &root;
  child2.parent = &root;
  child3.parent = &root;
  c31.parent = &child3;
  c32.parent = &child3;

  child3.children.emplace_back(&c31);
  child3.children.emplace_back(&c32);

  print_tree_from_node(&root, 0);
  */
  return 0;
}
