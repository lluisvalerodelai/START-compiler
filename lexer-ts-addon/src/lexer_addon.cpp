#include <napi.h>
#include "../../lexer/lexerpp.cpp"  // Include your lexer implementation

#include <sstream>

static Napi::Value CreateTokenArray(const Napi::CallbackInfo& info) {
	// We only need 1 argument
	if (info.Length() != 1) {
		Napi::Error::New(info.Env(), "Expected exactly one argument").ThrowAsJavaScriptException();
		return info.Env().Undefined();
	}

	// The argument should be the filename string
	if (!info[0].IsString()) {
		Napi::Error::New(info.Env(), "Expected a string argument").ThrowAsJavaScriptException();
		return info.Env().Undefined();
	}

	// Example vector: result of the lexer
	// std::vector<Token> cpparray = {
	// 	Token{token_type::comment, info[0].As<Napi::String>(), 333},
	// 	Token{token_type::identifier, "First token", 0},
	// 	Token{token_type::literal, "Second token", 100},
	// 	Token{token_type::END, "Last token", 101}
	// };

	std::istringstream content(info[0].As<Napi::String>(), std::istringstream::in | std::istringstream::binary);

	const std::vector<Token>& cpparray = lexer(content);

	// Return this c++ vector as a javasrcipt one
	Napi::Array jsarray = Napi::Array::New(info.Env(), cpparray.size());
	for (int i = 0; i < cpparray.size(); i++) {
		const Token& token = cpparray[i];
		Napi::Object jstoken = Napi::Object::New(info.Env());
		jstoken.Set("type", static_cast<int>(token.type));
		jstoken.Set("value", token.value);
		jstoken.Set("characterPosition", token.characterPosition);

		jsarray.Set(i, jstoken);
	}

	return jsarray;
}

static Napi::Object Init(Napi::Env env, Napi::Object exports) {
	exports.Set("CreateTokenArray", Napi::Function::New(env, CreateTokenArray));
	return exports;
}

NODE_API_MODULE(lexer_addon, Init);