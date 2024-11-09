#include <napi.h>
#include "lexer-test.cpp"  // Include your lexer implementation

Napi::Value LexerWrapper(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Wrong arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string file_path = info[0].As<Napi::String>().Utf8Value();
    lexer(file_path);  // Call your lexer function

    return Napi::String::New(env, "Lexing completed");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "lexer"), Napi::Function::New(env, LexerWrapper));
    return exports;
}

NODE_API_MODULE(lexer_addon, Init)