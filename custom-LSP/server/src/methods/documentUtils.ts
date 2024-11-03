import { Position, Range } from "vscode-languageserver";
import { TextDocument } from "vscode-languageserver-textdocument";

import { resolve } from "path";

export const packageRoot = resolve(__dirname, "..", "..", "..");

export type WordInfo = {
	word: string,
	range: Range
}

export function getWordAtPosition(document: TextDocument, position: Position): WordInfo | null {
	const line = document.getText({
		start: { line: position.line, character: 0 },
		end: { line: position.line, character: Number.MAX_VALUE }
	});

	const wordPattern = /\w+/g;
	let match;

	while ((match = wordPattern.exec(line))) {
		if (match.index <= position.character && position.character <= match.index + match[0].length) {
			return {
				word: match[0],
				range: {
					start: { line: position.line, character: match.index },
					end: { line: position.line, character: match.index + match[0].length },
				}
			}
		}
	}

	return null;
}