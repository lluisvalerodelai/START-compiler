"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const lexer_addon_1 = require("./lexer_addon");
// const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
// console.log("Path: ", testPath);
const result = (0, lexer_addon_1.lexer)("int main() {\r\n  // This is a comment\r\n  int helloWorld = 4;\r\n  string svar = \"hello!!!\";\r\n  \r\n  float fval = 100101110 * 1001;\r\n\r\n  return helloWorld;\r\n  float apple;\r\n  int x;\r\n}\r\n\r\nASD");
console.log("Results: ", result);
console.log(`Comment: ${JSON.stringify(result[6])}, Length: ${result[6].value.length}`);
//# sourceMappingURL=main.js.map