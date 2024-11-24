import path = require("path");
import { projectRoot } from "./projectRoot";
import { lexer, Token, token_type } from "./lexer_addon";

// const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
// console.log("Path: ", testPath);

const result: Token[] = lexer("int main() {\n  int helloWorld = 4;\n  string svar = \"hello!!!\";\n  \n  float fval = 100101110 * 1001;\n\n  return helloWorld;\n}");
console.log("Results: ", result);