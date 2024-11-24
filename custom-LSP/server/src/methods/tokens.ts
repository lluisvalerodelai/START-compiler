import { lexer, Token, token_type } from "lexer-ts-addon";
import path = require("path");
import { Diagnostic, DiagnosticSeverity } from "vscode-languageserver";
import { TextDocument, Position, Range } from "vscode-languageserver-textdocument";
import { URI } from "vscode-uri";

export type DocumentToken = {
	type: token_type;
	value: string;
	range: Range;
}

export const getPath = (textDocument: TextDocument): string => {
	return URI.parse(textDocument.uri).fsPath;
}

/**
 * Document path -> Tokens
 */
export const tokens: Map<string, DocumentToken[]> = new Map<string, DocumentToken[]>();

export const PrintSourceTokens = (docTokens: Token[]): void => {
	const printTokenObjects: Object[] = [];

	docTokens.forEach(token => {
		printTokenObjects.push({type: token_type[token.type], value: token.value, characterPosition: token.characterPosition});
		//console.log(`Token { type: ${token_type[token.type]}, value: ${token.value}, range: ${token.range} }`)
	});

	console.log(printTokenObjects);

	return;
}

export const PrintDocumentTokens = (docTokens: DocumentToken[]): void => {
	const printTokenObjects: Object[] = [];

	docTokens.forEach(token => {
		printTokenObjects.push({type: token_type[token.type], value: token.value, range: JSON.stringify(token.range)});
		//console.log(`Token { type: ${token_type[token.type]}, value: ${token.value}, range: ${token.range} }`)
	});

	console.log(printTokenObjects);

	return;
}

export const getTokenDoc = (textDocument: TextDocument, token: Token): string => {
	return `No documentation available for: ${token}`
}

/**
 * Converts a characterPosition coming from a file into an LSP Position
 */
export const characterposToPosition = (textDocument: TextDocument, characterPosition: number): Position => {
	const lines = textDocument.getText().split("\n");
	let charCount = 0;

	for (let line = 0; line < lines.length; line++) {
		const lineLength = lines[line].length + 1;
		if (characterPosition < charCount + lineLength) {
			return { line, character: characterPosition - charCount };
		}
		charCount += lineLength;
	}

	throw new Error("characterPosition exceeds text length");
}

export const tokenToRange = (textDocument: TextDocument, token: Token): Range => {
	const tokenStart = token.characterPosition - token.value.length + 1;

	return {
		start: characterposToPosition(textDocument, tokenStart),
		end: characterposToPosition(textDocument, token.characterPosition)
	}
}

export const convertToDocumentTokens = (textDocument: TextDocument, tokens: Token[]): DocumentToken[] => {
	return tokens.map(token => ({ type: token.type, value: token.value, range: tokenToRange(textDocument, token) }) as DocumentToken);
}

/**
 * 
 * @param uri URI of the file to be fetched from
 * @description Fetches and stores all tokens associated with a file URI in "tokens"
 */
export const fetchTokens = (textDocument: TextDocument): void => {
	const documentContent = textDocument.getText();

	const tokenArray: Token[] = lexer(documentContent);

	const docTokenArray: DocumentToken[] = convertToDocumentTokens(textDocument, tokenArray);

	tokens.set(textDocument.uri, docTokenArray);
}

export const validateTokens = (textDocument: TextDocument): Diagnostic[] => {
	const docTokens = tokens.get(textDocument.uri);

	if (!docTokens)
		return [{
			severity: DiagnosticSeverity.Warning,
			code: 0,
			range: { start: { line: 0, character: 0 }, end: { line: 0, character: 0 } },
			message: `No tokens found in document ${textDocument.uri}`,
			source: "Token Validation"
		}];

	const diagnostics: Diagnostic[] = [];

	// Generate diagnostics based on the tokens

	return diagnostics;
}