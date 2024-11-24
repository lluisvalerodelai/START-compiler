import { lexer, Token } from "lexer-ts-addon"

import path = require("path");
import { projectRoot } from "./methods/projectRoot";
import { convertToDocumentTokens, PrintDocumentTokens, PrintSourceTokens } from "./methods/tokens";
import { TextDocument } from "vscode-languageserver-textdocument";
import { URI } from "vscode-uri";

const content = "int main() {\n  int helloWorld = 4;\n  string svar = \"hello!!!\";\n  \n  float fval = 100101110 * 1001;\n\n  return helloWorld;\n}";

const testtextDoc = TextDocument.create(URI.file(path.resolve("./testSource")).fsPath, "languageId", 0.1, content);

const result: Token[] = lexer(content);
//console.log("Source tokens:")
//PrintSourceTokens(result);

const docToken = convertToDocumentTokens(testtextDoc, result);
PrintDocumentTokens(docToken);