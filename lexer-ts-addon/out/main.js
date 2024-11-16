"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const path = require("path");
const projectRoot_1 = require("./projectRoot");
const lexer_addon_1 = require("./lexer_addon");
const testPath = path.join(projectRoot_1.projectRoot, "..", "lexer", "tests", "valid", "test.c");
console.log("Path: ", testPath);
const result = (0, lexer_addon_1.lexer)(testPath);
console.log("Results: ", result);
//# sourceMappingURL=main.js.map