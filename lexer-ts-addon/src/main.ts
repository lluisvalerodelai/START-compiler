import path = require("path");
import { projectRoot } from "./projectRoot";
import { lexer, Token, token_type } from "./lexer_addon";

// const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
// console.log("Path: ", testPath);

const result: Token[] = lexer("int main() {\r\n  /* This\r\n   * is\r\n   * a\r\n   * multiline\r\n   * comment\r\n   */\r\n  int helloWorld = 44214214;\r\n  print(\"This is a string!\");\r\n\r\n  return helloWorld;\r\n}");
console.log("Results: ", result);

console.log(`Comment: ${JSON.stringify(result[6])}, Length: ${result[6].value.length}`);