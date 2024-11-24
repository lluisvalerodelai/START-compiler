import path = require("path");
import { projectRoot } from "./projectRoot";
import { lexer, Token, token_type } from "./lexer_addon";

// const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
// console.log("Path: ", testPath);

const result: Token[] = lexer("int main() {\n  int hello_world = 4;\n  string e = \"hello!!!\";\n\n  return 0;\n}");
console.log("Results: ", result);