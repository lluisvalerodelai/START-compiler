"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const lexer_addon_1 = require("./lexer_addon");
// const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
// console.log("Path: ", testPath);
const result = (0, lexer_addon_1.lexer)("int main() {\n  int hello_world = 4;\n  string e = \"hello!!!\";\n\n  return 0;\n}");
console.log("Results: ", result);
//# sourceMappingURL=main.js.map