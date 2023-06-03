/*
    Made by Livio Dal Maso (@Humeur) http://humeur.me/
*/

#include "node.h"

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "getDominantColors"), Napi::Function::New(env, Node::getDominantColors));
    exports.Set(Napi::String::New(env, "getDominantColorsRGB"), Napi::Function::New(env, Node::getDominantColorsRGB));
    exports.Set(Napi::String::New(env, "getDominantColorsHSV"), Napi::Function::New(env, Node::getDominantColorsHSV));

    return exports;
}

namespace Node {

Args handleArguments(const Napi::CallbackInfo &info, char *success) {
    Napi::Env env = info.Env();
    bool enlivenOptional;
    Args args;

    if (info.Length() == 2) {
        enlivenOptional = true;
    } else if (info.Length() == 3) {
        if (!info[2].IsBoolean()) {
            Node::ArgumentsError(env, "enliven argument must be a boolean.")
                    .ThrowAsJavaScriptException();
            return args;
        }
        enlivenOptional = info[2].As<Napi::Boolean>().ToBoolean();
    } else {
        Node::ArgumentsError(env, "This function takes 3 arguments: [imagePath {string}, dominantCount {int}, enliven = true {bool}].")
                .ThrowAsJavaScriptException();
        return args;
    }

    if (!info[0].IsString()) {
        Node::ArgumentsError(env, "imagePath argument must be a string.")
                .ThrowAsJavaScriptException();
        return args;
    }
    if (!info[1].IsNumber()) {
        Node::ArgumentsError(env, "dominantCount must be an Integer.")
                .ThrowAsJavaScriptException();
        return args;
    }

    args = {
            info[0].As<Napi::String>().ToString(),
            info[1].As<Napi::Number>().Int32Value(),
            enlivenOptional,
    };

    std::ifstream image(args.imagePath);
    if (!image.is_open()) {
        Node::ArgumentsError(env, "imagePath argument is not a valid path to an image.")
                .ThrowAsJavaScriptException();
        return args;
    }

    if (args.dominantCount < 1 || args.dominantCount > 10) {
        Node::ArgumentsError(env, "dominantCount must be between 1 and 10.")
                .ThrowAsJavaScriptException();
        return args;
    }

    *success = 1;
    return args;
}

Napi::Value getDominantColors(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    char success;
    Args args = handleArguments(info, &success);

    if (!success) {
        return env.Null();
    }

    return getDominantColorsImpl<DominantColor::HexColor>(info, args, HEX, env);
}

Napi::Value getDominantColorsRGB(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    char success;
    Args args = handleArguments(info, &success);

    if (!success) {
        return env.Null();
    }

    return getDominantColorsImpl<DominantColor::RGB>(info, args, RGB, env);
}

Napi::Value getDominantColorsHSV(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    char success;
    Args args = handleArguments(info, &success);

    if (!success) {
        return env.Null();
    }

    return getDominantColorsImpl<DominantColor::HSV>(info, args, HSV, env);
}
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)