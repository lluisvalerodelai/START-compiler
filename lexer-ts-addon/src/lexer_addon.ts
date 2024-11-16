const binding = require("bindings")("lexer_addon")

export enum token_type {
	identifier,
	keyword,
	separator,
	operator_token,
	literal,
	END,
	comment,
}

type ObjectToken = {
	type: number,
	value: string,
	characterPosition: number
}

export type Token = {
	type: token_type,
	value: string,
	characterPosition: number
};

export const lexer = (file_name: string): Token[] => {
	const result = binding.CreateTokenArray(file_name) as ObjectToken[];

	return result.map(obj => {
        // Ensure the type is valid in the enum
        if (!(obj.type in token_type)) {
            throw new Error(`Invalid type value: ${obj.type}`);
        }

        return {
            type: obj.type as token_type, // Cast to token_type
            value: obj.value,
            characterPosition: obj.characterPosition
        };
    });
};