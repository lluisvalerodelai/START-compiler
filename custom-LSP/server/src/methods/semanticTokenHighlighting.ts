import {
	SemanticTokens,
	SemanticTokensBuilder,
	SemanticTokensLegend,
	SemanticTokenTypes,
	TextDocumentIdentifier,
} from 'vscode-languageserver/node';
import { documents, showEditorMessage } from "../server"
import { DocumentToken, getPath, tokens } from './tokens';
import { TextDocument } from 'vscode-languageserver-textdocument';
import { token_type } from 'lexer-ts-addon';

// export declare enum token_type {
//     identifier = 0,
//     keyword = 1,
//     separator = 2,
//     operator_token = 3,
//     literal = 4,
//     END = 5,
//     comment = 6
// }

const legendTokenTypes = [
	SemanticTokenTypes.variable, // Identifier
	SemanticTokenTypes.keyword, // Keyword
	SemanticTokenTypes.operator, // separator, operator_token
	SemanticTokenTypes.number, // int, string LITERAL
	SemanticTokenTypes.comment
];

export const legend: SemanticTokensLegend = {
	tokenTypes: legendTokenTypes,
	tokenModifiers: []
};

export const mapTokenType = (documentToken: DocumentToken): SemanticTokenTypes => {
	switch (documentToken.type) {
		case token_type.identifier:
			return legendTokenTypes[0];
		case token_type.keyword:
			return legendTokenTypes[1];
		case token_type.separator:
		case token_type.operator_token:
			return legendTokenTypes[2];
		case token_type.literal:
			return legendTokenTypes[3];
		case token_type.comment:
			return legendTokenTypes[4];
	}

	throw Error(`Tried to map unknown token type: ${JSON.stringify(documentToken)}`);
}

export const semanticTokensFull = (textDocument: TextDocument): SemanticTokens | null => {
	const document: TextDocument | undefined = documents.get(textDocument.uri);
	if (document === undefined) {
		showEditorMessage(`Trouble opening document ${textDocument.uri}}`);
		return null;
	}
	const lines = document.getText().split("\n");

	const builder = new SemanticTokensBuilder();

	const docTokens = tokens.get(textDocument.uri);

	if (docTokens === undefined) {
		showEditorMessage(`No teokens found in document ${textDocument.uri}}`);
		return null;
	}

	for (const token of docTokens) {
		if (token.value.length === 0 || token.type === token_type.END)
			continue;

		const tokenType = mapTokenType(token);
		const tokenTypeIndex = legendTokenTypes.indexOf(tokenType);

		// If the token spans multiple lines, we have to build it line by line
		if (token.range.start.line === token.range.end.line) {
			builder.push(
				token.range.start.line, // Each token only spans at most 1 line
				token.range.start.character,
				token.value.length,
				tokenTypeIndex,
				0
			);
		}
		else {
			const tokenLines = token.value.split("\n");
			// First line
			//12345
			const startTokenLen = lines[token.range.start.line].length - token.range.start.character;
			builder.push(token.range.start.line, token.range.start.character, startTokenLen, tokenTypeIndex, 0);
			// Middle lines
			for (let i: number = 1; i < tokenLines.length - 1; i++) {
				const tokenLen = lines[token.range.start.line + i].length;
				builder.push(token.range.start.line + i, 0, tokenLen, tokenTypeIndex, 0);
			}
			// Last line
			builder.push(token.range.end.line, 0, token.range.end.character + 1, tokenTypeIndex, 0);
		}

	}

	return builder.build();
};