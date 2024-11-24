import { Position, Range } from "vscode-languageserver";
import { TextDocument } from "vscode-languageserver-textdocument";

import path = require("path");
import { projectRoot } from "./projectRoot";
import { DocumentToken } from "./tokens";

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

export const formatPosition = (position: Position): string => {
	position.line += 1;
	position.character += 1;

	return JSON.stringify(position);
}

export const NULL_POSITION: Position = {
	line: 0,
	character: 0
}

export const EMPTY_RANGE: Range = {
	start: NULL_POSITION,
	end: NULL_POSITION
}