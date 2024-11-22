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
export declare const lexer: (file_name: string) => Token[];
