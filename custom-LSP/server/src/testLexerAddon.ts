import { lexer, Token } from "lexer-ts-addon"

import path = require("path");
import { projectRoot } from "./methods/projectRoot";
import { convertToDocumentTokens, PrintDocumentTokens, PrintSourceTokens } from "./methods/tokens";
import { TextDocument } from "vscode-languageserver-textdocument";
import { URI } from "vscode-uri";
import fs = require("fs");

const testPath = path.join(projectRoot, "..", "lexer", "tests", "valid", "test.c");
console.log("Path: ", testPath);

const testtextDoc = TextDocument.create(URI.file(testPath).fsPath, "languageIs", 0.1, fs.readFileSync(testPath).toString());

const result: Token[] = lexer("int main() {\n  int hello_world = 4;\n  string e = \"hello!!!\";\n\n  return 0;\n}");
//console.log("Results: ", result);

const docToken = convertToDocumentTokens(testtextDoc, result);
PrintDocumentTokens(docToken);