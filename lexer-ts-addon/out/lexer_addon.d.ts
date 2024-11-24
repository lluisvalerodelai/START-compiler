export declare enum token_type {
    identifier = 0,
    keyword = 1,
    separator = 2,
    operator_token = 3,
    literal = 4,
    END = 5,
    comment = 6
}
export type Token = {
    type: token_type;
    value: string;
    characterPosition: number;
};
/**
 *
 * @param file_content The content of the file which should be parsed
 * @returns Array of tokens found in the content
 */
export declare const lexer: (file_content: string) => Token[];
