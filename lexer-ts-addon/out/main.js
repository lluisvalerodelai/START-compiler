"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const lexer_addon_1 = require("./lexer_addon");
// const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
// console.log("Path: ", testPath);
const result = (0, lexer_addon_1.lexer)("int main() {\r\n  /* This\r\n   * is\r\n   * a\r\n   * multiline\r\n   * comment\r\n   */\r\n  int helloWorld = 44214214;\r\n  print(\"This is a string!\");\r\n\r\n  return helloWorld;\r\n}");
console.log("Results: ", result);
console.log(`Comment: ${JSON.stringify(result[6])}, Length: ${result[6].value.length}`);
//# sourceMappingURL=main.js.map