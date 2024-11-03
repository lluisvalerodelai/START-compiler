import { readFileSync } from "fs";
import { join } from "path";
import { packageRoot } from "./documentUtils";
import log from "./log";

export interface Token {
	type: number;
	value: string;
	characterPosition: number;
}

const jsonString = readFileSync(join(packageRoot, "lexer_out", "tokens.json"), "utf-8");
export const tokens: Token[] = JSON.parse(jsonString);

//DEBUG
log.write(tokens);