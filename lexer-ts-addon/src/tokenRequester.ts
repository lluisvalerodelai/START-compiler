import * as fs from "fs";
import { projectRoot } from "./projectRoot";
const path = require('path');

const lexerAddon = require(path.join(projectRoot, "build", "Release", "lexer_addon"));
const tokenPath = path.join(projectRoot, "lexer_out", "tokens.json");

export type Token = {
	type: number,
	value: string,
	characterPosition: number
};

export function requestTokens(filePath: string): Promise<Token[]> {
	return new Promise((resolve, reject) => {
		try {
			lexerAddon.lexer(filePath);

			// Read the tokens from the JSON file
			fs.readFile(tokenPath, 'utf8', (err, data) => {
				if (err) {
					reject(err);
				} else {
					const tokens: Token[] = JSON.parse(data);
					resolve(tokens);
				}
			});
		} catch (error) {
			reject(error);
		}
	});
}