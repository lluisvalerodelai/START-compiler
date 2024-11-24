"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.lexer = exports.token_type = void 0;
const binding = require("bindings")("lexer_addon");
var token_type;
(function (token_type) {
    token_type[token_type["identifier"] = 0] = "identifier";
    token_type[token_type["keyword"] = 1] = "keyword";
    token_type[token_type["separator"] = 2] = "separator";
    token_type[token_type["operator_token"] = 3] = "operator_token";
    token_type[token_type["literal"] = 4] = "literal";
    token_type[token_type["END"] = 5] = "END";
    token_type[token_type["comment"] = 6] = "comment";
})(token_type || (exports.token_type = token_type = {}));
/**
 *
 * @param file_content The content of the file which should be parsed
 * @returns Array of tokens found in the content
 */
const lexer = (file_content) => {
    const result = binding.CreateTokenArray(file_content);
    return result.map(obj => {
        // Ensure the type is valid in the enum
        if (!(obj.type in token_type)) {
            throw new Error(`Invalid type value: ${obj.type}`);
        }
        return {
            type: obj.type, // Cast to token_type
            value: obj.value,
            characterPosition: obj.characterPosition
        };
    });
};
exports.lexer = lexer;
//# sourceMappingURL=lexer_addon.js.map