/*
    Made by Livio Dal Maso (@Humeur) http://humeur.me/
*/

#pragma once

#include <fstream>

#include "../node_modules/node-addon-api/napi.h"

#include "DominantColor.h"

namespace Node {
    class ArgumentsError : public Napi::Error {
        public:
            ArgumentsError(Napi::Env env, const char *msg) : Napi::Error(env, Napi::String::New(env, msg)) {}
    };

    enum CHANNEL : int {
        HEX,
        RGB,
        HSV
    };

    struct Args {
        std::string imagePath;
        int dominantCount;
        bool enliven;
    };

    template <typename Type>
    std::vector<Type> getDominantColorsTyped(Args args);

    template <typename Type>
    Napi::Value createValueArray(std::vector<Type> dominantColors, Napi::Env env);

    Node::Args handleArguments(const Napi::CallbackInfo& info, char *success);
    Napi::Value getDominantColors(const Napi::CallbackInfo& info);
    Napi::Value getDominantColorsRGB(const Napi::CallbackInfo& info);
    Napi::Value getDominantColorsHSV(const Napi::CallbackInfo& info);

    template <typename Type>
    Napi::Value getDominantColorsImpl(const Napi::CallbackInfo &info, Args args, CHANNEL channel, Napi::Env env) {
        std::vector<Type> dominantColors = getDominantColorsTyped<Type>(args);

        return createValueArray<Type>(dominantColors, env);
    }

    template <>
    std::vector<DominantColor::HexColor> getDominantColorsTyped<DominantColor::HexColor>(Args args) {
        return DominantColor::get<DominantColor::HexColor>(args.imagePath, args.dominantCount, args.enliven);
    }

    template <>
    std::vector<DominantColor::RGB> getDominantColorsTyped<DominantColor::RGB>(Args args) {
        return DominantColor::get<DominantColor::RGB>(args.imagePath, args.dominantCount, args.enliven);
    }

    template <>
    std::vector<DominantColor::HSV> getDominantColorsTyped<DominantColor::HSV>(Args args) {
        return DominantColor::get<DominantColor::HSV>(args.imagePath, args.dominantCount, args.enliven);
    }

    template <>
    Napi::Value createValueArray<DominantColor::HexColor>(std::vector<DominantColor::HexColor> dominantColors, Napi::Env env) {
        Napi::Array result = Napi::Array::New(env, dominantColors.size());

        for (int i = 0 ; i < dominantColors.size() ; ++i) result[i] = Napi::String::New(env, dominantColors[i]);

        return result;
    }

    template <>
    Napi::Value createValueArray<DominantColor::RGB>(std::vector<DominantColor::RGB> dominantColors, Napi::Env env) {
        Napi::Array result = Napi::Array::New(env, dominantColors.size() * 3);

        int i = 0;
        for (const auto &dominantColor : dominantColors) {
            result[i] = Napi::Number::New(env, dominantColor.r);
            result[i + 1] = Napi::Number::New(env, dominantColor.g);
            result[i + 2] = Napi::Number::New(env, dominantColor.b);
            i += 3;
        }

        return result;
    }

    template <>
    Napi::Value createValueArray<DominantColor::HSV>(std::vector<DominantColor::HSV> dominantColors, Napi::Env env) {
        Napi::Float64Array result = Napi::Float64Array::New(env, dominantColors.size() * 3);

        int i = 0;
        for (const auto &dominantColor : dominantColors) {
            result[i] = Napi::Number::New(env, dominantColor.h);
            result[i + 1] = Napi::Number::New(env, dominantColor.s);
            result[i + 2] = Napi::Number::New(env, dominantColor.v);
            i += 3;
        }

        return result;
    }
}