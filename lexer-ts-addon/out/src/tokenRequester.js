"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || function (mod) {
    if (mod && mod.__esModule) return mod;
    var result = {};
    if (mod != null) for (var k in mod) if (k !== "default" && Object.prototype.hasOwnProperty.call(mod, k)) __createBinding(result, mod, k);
    __setModuleDefault(result, mod);
    return result;
};
Object.defineProperty(exports, "__esModule", { value: true });
exports.requestTokens = requestTokens;
const fs = __importStar(require("fs"));
const projectRoot_1 = require("./projectRoot");
const path = require('path');
const lexerAddon = require(path.join(projectRoot_1.projectRoot, "build", "Release", "lexer_addon"));
const tokenPath = path.join(projectRoot_1.projectRoot, "lexer_out", "tokens.json");
function requestTokens(filePath) {
    return new Promise((resolve, reject) => {
        try {
            lexerAddon.lexer(filePath);
            // Read the tokens from the JSON file
            fs.readFile(tokenPath, 'utf8', (err, data) => {
                if (err) {
                    reject(err);
                }
                else {
                    const tokens = JSON.parse(data);
                    resolve(tokens);
                }
            });
        }
        catch (error) {
            reject(error);
        }
    });
}
//# sourceMappingURL=tokenRequester.js.map