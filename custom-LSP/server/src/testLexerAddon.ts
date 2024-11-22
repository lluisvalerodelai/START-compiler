import { lexer, Token } from "lexer-ts-addon"

import path = require("path");
import { projectRoot } from "./methods/projectRoot";

const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
console.log("Path: ", testPath);

const result: Token[] = lexer(testPath);
console.log("Results: ", result);