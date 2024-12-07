import {
	createConnection,
	TextDocuments,
	ProposedFeatures,
	InitializeParams,
	TextDocumentSyncKind,
	InitializeResult,
	CompletionItemKind,
	Diagnostic,
	DiagnosticSeverity,
	MarkupKind,
	CompletionParams,
	CompletionItem,
	CompletionList,
	InsertTextMode,
	InsertReplaceEdit,
	HoverParams,
	Hover,
	DefinitionParams,
	Definition,
	LocationLink,
	combineWindowFeatures,
} from "vscode-languageserver/node";

import { Range, TextDocument } from "vscode-languageserver-textdocument";
import { getWordAtPosition, packageRoot } from "./methods/documentUtils";
import { join } from "path";
import { existsSync } from "fs";
import { URI } from "vscode-uri"

// Create a connection for the server, using Node's IPC as a transport.
// Also include all preview / proposed LSP features.
const connection = createConnection(ProposedFeatures.all);

// Create a simple text document manager.
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

// initialize
connection.onInitialize((params: InitializeParams): InitializeResult => {
	const result: InitializeResult = {
		// List out all capabilities
		capabilities: {
			textDocumentSync: TextDocumentSyncKind.Incremental,
			completionProvider: {
				resolveProvider: true,
				triggerCharacters: ['.']
			},
			hoverProvider: true,
			definitionProvider: true,
		},
	};

	return result;
});

// textDocument/completion
connection.onCompletion((completionParams: CompletionParams): CompletionList | null => {
	const document = documents.get(completionParams.textDocument.uri);

	const completionList: CompletionList = {
		isIncomplete: false,
		items: [
			{
				label: 'TypeScript',
				kind: CompletionItemKind.Text,
				data: completionParams // Data which can be passed around onCompletion -> onCompletionResolve
			},
			// Add more completion items as needed
		]
	};

	if (document) {
		const wordInfo = getWordAtPosition(document, completionParams.position);

		if (wordInfo) {

			completionList.items.push({
				label: wordInfo.word,
				textEdit: {
					range: wordInfo.range, // The range where newText will be inserted
					newText: `--${wordInfo.word}--`,

				},
				kind: CompletionItemKind.Variable,
				data: completionParams,
				detail: "This is the word you are hovering over",
			});
		}
	}

	// Implement your completion logic here
	return completionList;
});

// When hovering over a completion item, it displays some info about it
connection.onCompletionResolve((item: CompletionItem): CompletionItem => {
	const params = item.data as CompletionParams;

	// Optionally provide additional information for the selected item
	if (item.label === "TypeScript") {
		item.detail = 'This is some information about this item';
		item.insertText = 'This text will be ignored because textEdit overrides it';
		item.textEdit = {
			range: { start: params.position, end: params.position },
			newText: "This will be actually inserted",
		}
		item.kind = CompletionItemKind.Interface;
		item.insertTextMode = InsertTextMode.adjustIndentation;
		item.documentation = 'TypeScript documentation';
	}
	return item;
});

documents.onDidChangeContent((change) => {
	validateTextDocument(change.document);
});

function validateTextDocument(textDocument: TextDocument): void {
	const diagnostics: Diagnostic[] = [];
	// Implement your diagnostic logic here
	// Example:
	const text = textDocument.getText();
	const pattern = /\b[A-Z]{2,}\b/g;
	let match;

	// Matches all capital words with minimum of 2 characters
	while ((match = pattern.exec(text))) {
		diagnostics.push({
			severity: DiagnosticSeverity.Warning,
			code: 69,
			range: {
				start: textDocument.positionAt(match.index),
				end: textDocument.positionAt(match.index + match[0].length)
			},
			message: `${match[0]} is all uppercase.`,
			source: 'example diagnostics',
			/**
			 * A data entry field that is preserved between a
			 * `textDocument/publishDiagnostics` notification and
			 * `textDocument/codeAction` request.
			 * 
			 * ex: diagnostics finds suggestions, codeAction executes them
			 */
			data: "Diagnostics data",
		});
	}

	connection.sendDiagnostics({ uri: textDocument.uri, version: textDocument.version, diagnostics: diagnostics });
}

// Mouse hovers over or ctrl+k,ctrl+i
connection.onHover((params: HoverParams): Hover | null => {
	const document = documents.get(params.textDocument.uri);

	if (!document) {
		connection.window.showWarningMessage(`Could not open document ${params.textDocument.uri}...`);
		return null;
	}

	const position = params.position;

	// Implement your hover logic here
	// Example:
	const wordInfo = getWordAtPosition(document, position);

	if (!wordInfo) {
		connection.window.showWarningMessage(`Could not find the symbol hovered over at position (line: ${params.position.line}, character: ${params.position.character}...`);

		return {
			contents: {
				kind: MarkupKind.Markdown,
				value: 'Hover is empty'
			}
		};
	}

	if (wordInfo.word === 'TypeScript') {
		return {
			contents: {
				kind: MarkupKind.Markdown,
				value: '**TypeScript** is a typed superset of JavaScript that compiles to plain JavaScript.'
			}
		};
	}
	else {
		return {
			contents: {
				kind: MarkupKind.Markdown,
				value: 'No hover definition found'
			}
		};
	}

	return null;
});


connection.onDefinition((params: DefinitionParams): LocationLink[] | null => {
	const document = documents.get(params.textDocument.uri);

	if (!document) {
		connection.window.showWarningMessage(`Could not open document ${params.textDocument.uri}...`);
		return null;
	}

	const position = params.position;
	// Implement your jump to definition logic here
	// Example:
	const wordInfo = getWordAtPosition(document, position);

	if (!wordInfo) {
		connection.window.showWarningMessage(`Could not find the symbol which requested definition at position (line: ${params.position.line}, character: ${params.position.character}...`);
		return null;
	}

	if (wordInfo.word === 'exampleFunction') {
		const testDefinitionFile = URI.file(join(packageRoot, "testDefinitionFile.txt")).toString();

		const testDefinitionRange: Range = {
			start: { line: 1, character: 0 },
			end: { line: 2, character: 5 }
		};

		if (existsSync(testDefinitionFile)) {
			connection.window.showInformationMessage(`Found definition file: ${testDefinitionFile}`);
		}

		return [{
			originSelectionRange: wordInfo.range, // Range of the symbol (in original file) which was clicked on
			targetUri: testDefinitionFile, // In which document the definition is
			targetRange: testDefinitionRange, // Full range of definition of the symbol in targetUri, contains: comments, symbol definition
			targetSelectionRange: testDefinitionRange // A more precise range within the targetRange that pinpoints the defined symbol.
		}];
	}

	connection.window.showWarningMessage(`Could not find any definition for \"${wordInfo.word}\"`);
	return null;
});

// Make the text document manager listen on the connection
// for open, change and close text document events
documents.listen(connection);

// Listen on the connection
connection.listen();
