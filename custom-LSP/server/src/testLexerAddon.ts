import { lexer, Token } from "lexer-ts-addon"

import path = require("path");
import { projectRoot } from "./methods/projectRoot";
import { characterposToPosition, convertToDocumentTokens, DocumentToken, findIdentifierDefinitions, PrintDocumentTokens, PrintSourceTokens } from "./methods/tokens";
import { TextDocument } from "vscode-languageserver-textdocument";
import { URI } from "vscode-uri";

const content = `int main() {\r\n  /* This\r\n   * is\r\n   * a\r\n   * multiline\r\n   * comment\r\n   */\r\n  int helloWorld = 44214214;\r\n  print("This is a string!");\r\n\r\n  return helloWorld;\r\n}`

const testtextDoc = TextDocument.create(URI.file(path.resolve("./testSource")).fsPath, "languageId", 0.1, content);

const result: Token[] = lexer(content);
//console.log("\x1b[31m Source Tokens: \x1b[0m")
//PrintSourceTokens(result);

const docToken: DocumentToken[] = convertToDocumentTokens(testtextDoc, result);
console.log("\x1b[31m DocumentTokens: \x1b[0m");
PrintDocumentTokens(docToken);

const testValue = characterposToPosition(testtextDoc, 16);
console.log(`\x1b[31m testValue: ${JSON.stringify(testValue)} \x1b[0m`);
console.log(`\x1b[31m testLen: ${"int main() {\r".length} \x1b[0m`);


// const identifierDefinitions: DocumentToken[] = findIdentifierDefinitions(testtextDoc, docToken);
// console.log("\x1b[31mIdentifier definitions: \x1b[0m");
// PrintDocumentTokens(identifierDefinitions);