import { lexer, Token, token_type } from "lexer-ts-addon";
import path = require("path");
import { DefinitionParams, Diagnostic, DiagnosticSeverity, HoverParams, LocationLink, MarkupContent, MarkupKind } from "vscode-languageserver";
import { TextDocument, Position, Range } from "vscode-languageserver-textdocument";
import { URI } from "vscode-uri";
import { documents, showEditorMessage } from "../server";
import { EMPTY_RANGE } from "./documentUtils";
import log from "./log";

export type DocumentToken = {
	type: token_type;
	value: string;
	range: Range;
}

export type DocumentTokenDocumentation = DocumentToken & {
	doc: DocumentToken;
}

export const typeToString = (type: token_type): string => { return token_type[type] };

export const formatToken = (token: Token): string => {
	const printObj: Object = { type: typeToString(token.type), value: token.value, characterPosition: token.characterPosition };

	return JSON.stringify(printObj);
}
export const formatDToken = (token: DocumentToken): string => {
	const printObj: Object = { type: typeToString(token.type), value: token.value, range: JSON.stringify(token.range) };

	return JSON.stringify(printObj);
}

export const getPath = (textDocument: TextDocument): string => {
	return URI.parse(textDocument.uri).fsPath;
}

/**
 * Key: textDocument.uri
 * Value: DocumentToken[]
 */
export const tokens: Map<string, DocumentToken[]> = new Map<string, DocumentToken[]>();
/**
 * Key: textDocument.uri
 * Value: Identifier definition tokens along with their documentations
 */
export const identifierTokenDefinitions: Map<string, DocumentTokenDocumentation[]> = new Map<string, DocumentTokenDocumentation[]>();

export const PrintSourceTokens = (docTokens: Token[]): void => {
	const printTokenObjects: Object[] = [];

	docTokens.forEach(token => {
		printTokenObjects.push({ type: typeToString(token.type), value: JSON.stringify(token.value), characterPosition: token.characterPosition, length: token.value.length });
	});

	console.log(printTokenObjects);

	return;
}

export const PrintDocumentTokens = (docTokens: DocumentToken[]): void => {
	const printTokenObjects: Object[] = [];

	docTokens.forEach(token => {
		printTokenObjects.push({ type: typeToString(token.type), value: token.value, range: JSON.stringify(token.range) });
	});

	console.log(printTokenObjects);

	return;
}

/**
 * 
 * @param textDocument 
 * @param token 
 * @returns Markdown formatted string as token documentation
 */
export const getTokenDoc = (textDocument: TextDocument, token: DocumentToken): DocumentTokenDocumentation => {
	// First look at the line above for comments
	const commentLine: Range = { start: { line: token.range.start.line - 1, character: 0 }, end: { line: token.range.start.line - 1, character: Number.MAX_SAFE_INTEGER } };
	// See if there are any comments
	const commentToken: DocumentToken | undefined = tokens.get(textDocument.uri)?.find(token => token.type === token_type.comment && doesRangeIntersect(commentLine, token.range));

	const tokenWithDocumentation: DocumentTokenDocumentation = {
		type: token.type,
		value: token.value,
		range: token.range,
		doc: { type: token_type.comment, value: `No documentation available for: ${formatDToken(token)}`, range: EMPTY_RANGE }
	}

	if (commentToken === undefined)
		return tokenWithDocumentation;

	tokenWithDocumentation.doc = commentToken;

	return tokenWithDocumentation;
}

/**
 * Converts a characterPosition coming from a file into an LSP Position
 */
export const characterposToPosition = (textDocument: TextDocument, characterPosition: number): Position => {
	const lines = textDocument.getText().split("\n");
	let charCount = 0;

	for (let line = 0; line < lines.length; line++) {
		const lineLength = lines[line].length + 1;

		if (characterPosition < charCount + lineLength)
			return { line, character: characterPosition - charCount };

		charCount += lineLength;
	}

	throw new Error("characterPosition exceeds text length");
}

export const positionToRange = (position: Position): Range => {
	return {
		start: position,
		end: position
	}
}

export const tokenToRange = (textDocument: TextDocument, token: Token): Range => {
	const tokenStart = token.characterPosition - token.value.length + 1;

	const range: Range = {
		start: characterposToPosition(textDocument, tokenStart),
		end: characterposToPosition(textDocument, token.characterPosition)
	}
	return range;
}

export const isPositionInRange = (position: Position, range: Range, extended: boolean = false): boolean => {
	if (position.line < range.start.line || position.line > range.end.line)
		return false;

	if (position.line === range.start.line && position.character < range.start.character)
		return false;

	if (position.line === range.end.line && position.character > range.end.character + Number(extended))
		return false;

	return true;
}

export const doesRangeIntersect = (A: Range, B: Range): boolean => {
	if (isPositionInRange(B.start, A) || isPositionInRange(B.end, A) || isPositionInRange(A.start, B) || isPositionInRange(A.end, B))
		return true;

	return false;
}

export const convertToDocumentTokens = (textDocument: TextDocument, tokens: Token[]): DocumentToken[] => {
	return tokens.map(token => ({ type: token.type, value: token.value, range: tokenToRange(textDocument, token) }) as DocumentToken);
}

export const findIdentifierDefinitions = (textDocument: TextDocument, docTokens: DocumentToken[]): DocumentTokenDocumentation[] => {
	const identifierNames: string[] = [];
	const definitions: DocumentTokenDocumentation[] = [];

	docTokens.forEach(token => {
		if (token.type === token_type.identifier && !identifierNames.includes(token.value)) {
			identifierNames.push(token.value);
			definitions.push(getTokenDoc(textDocument, token));
		}
	});

	return definitions;
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
	PrintDocumentTokens(docTokenArray);

	tokens.set(textDocument.uri, docTokenArray);

	// Set up documentation info
	identifierTokenDefinitions.set(textDocument.uri, findIdentifierDefinitions(textDocument, docTokenArray));
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

export const getHoverInfo = (params: HoverParams, token: DocumentToken): MarkupContent => {
	// This is the markdown that's going to be shown
	const document = documents.get(params.textDocument.uri);
	if (document === undefined)
		return { kind: MarkupKind.Markdown, value: `No definition found...  \nTrouble opening: ${params.textDocument.uri}` };

	const identifierDefinitions: DocumentTokenDocumentation[] | undefined = identifierTokenDefinitions.get(params.textDocument.uri);
	if (identifierDefinitions === undefined)
		return { kind: MarkupKind.Markdown, value: `No definition found...  \nNo definitions found in file: ${params.textDocument.uri}` };

	const definition: DocumentTokenDocumentation | undefined = identifierDefinitions.find(def => def.value === token.value);
	if (definition === undefined)
		return { kind: MarkupKind.Markdown, value: `**${token.value}**: *${typeToString(token.type)}*  \nNo definitions found for token...` };

	const content = `**${definition.value}**: *${typeToString(definition.type)}*  \nDoc range:  \n${JSON.stringify(definition.doc.range)}  \n**Doc:**:  \n${definition.doc.value}`;

	const markupContent: MarkupContent = {
		kind: MarkupKind.Markdown,
		value: content
	}

	return markupContent;
}

const defaultLocationLink = (uri: string, token: DocumentToken): LocationLink => {
	return LocationLink.create(uri, token.range, token.range, token.range);
}
const definitionRangeFull = (token: DocumentTokenDocumentation): Range => {
	return {
		start: token.doc.range.start,
		end: token.range.end
	}
}

export const getDefinitionInfo = (params: DefinitionParams, token: DocumentToken): LocationLink[] => {
	const returnIfUndefined = [defaultLocationLink(params.textDocument.uri, token)];
	const document = documents.get(params.textDocument.uri);
	if (document === undefined)
		return returnIfUndefined;

	const identifierDefinitions: DocumentTokenDocumentation[] | undefined = identifierTokenDefinitions.get(params.textDocument.uri);
	if (identifierDefinitions === undefined)
		return returnIfUndefined;

	const definition: DocumentTokenDocumentation | undefined = identifierDefinitions.find(def => def.value === token.value);
	if (definition === undefined)
		return returnIfUndefined;

	// targetUri: In which document the definition is
	// targetRange: Full range of definition of the symbol in targetUri, contains: comments, symbol definition
	// targetSelectionRange: A more precise range within the targetRange that pinpoints the defined symbol.
	// originSelectionRange?: Range of the symbol (in original file) which was clicked on
	return [LocationLink.create(params.textDocument.uri, definitionRangeFull(definition), definition.range, token.range)];
}