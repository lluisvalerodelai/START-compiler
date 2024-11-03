#include "token_to_json.hpp"

const std::filesystem::path JsonTokenFile = std::filesystem::current_path() /  std::filesystem::path("../../custom-LSP/lexer_out/tokens.json");

std::string escapeJsonString(const std::string &input)
{
	std::ostringstream oss;
	for (char c : input)
	{
		switch (c)
		{
		case '"':
			oss << "\\\"";
			break;
		case '\\':
			oss << "\\\\";
			break;
		case '\b':
			oss << "\\b";
			break;
		case '\f':
			oss << "\\f";
			break;
		case '\n':
			oss << "\\n";
			break;
		case '\r':
			oss << "\\r";
			break;
		case '\t':
			oss << "\\t";
			break;
		default:
			if ('\x00' <= c && c <= '\x1f')
			{
				oss << "\\u" << std::hex << std::setw(4) << std::setfill('0') << static_cast<int>(c);
			}
			else
			{
				oss << c;
			}
		}
	}
	return oss.str();
}

std::string ToJSON(const Token &token)
{
	std::ostringstream oss;
	oss << "{\"type\":" << token.type << ","
		<< "\"value\":\"" << escapeJsonString(token.value) << "\","
		<< "\"characterPosition\":" << token.characterPosition << "}";
	return oss.str();
}

void ToJSON(const std::vector<Token>& tokens)
{
	std::ofstream f(JsonTokenFile, std::ios::out);

	if (!f.is_open()) {
        std::cerr << "Unable to open file: " << JsonTokenFile << std::endl;
        return;
    }

	f << "[\n";
	for (int i = 0; i < tokens.size(); ++i) {
        f << "  " << ToJSON(tokens[i]);
        if (i < tokens.size() - 1) {
            f << ",";
        }
        f << "\n";
    }
    f << "]" << std::endl;
}