/*
    Made by Livio Dal Maso (@Humeur) http://humeur.me/
*/

#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <memory>
#include <utility>
#include <iomanip>
#include <sstream>
#include <stdexcept>

#include "PeakFinder.h"

namespace DominantColor {
    using HexColor = std::string;
    constexpr int MAX_SIZE = 65536;

    template <typename Type>
    struct ColorConverter;

    struct RGB {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    };

    struct HSV {
        double h;
        double s;
        double v;
    };

    struct pixel {
        int count;
        double sAverage;
        double vAverage;
    };

    bool double_equals(double a, double b, double epsilon = 0.00001);
    HSV RGBToHSV(const RGB &pixel);
    RGB HSVToRGB(const HSV &pixel);
    std::string RGBToHex(const RGB &pixel);
    void getNewSizeFromMaxSize(int *originalWidth, int *originalHeight);
    std::unique_ptr<unsigned char[]> resizeImage(std::unique_ptr<unsigned char[]> imageSrc, int *width, int *height);
    std::array<DominantColor::pixel, 360> getPixelsOccurencesFromImagePath(std::string imagePath);
    std::vector<std::pair<pixel, int>> getPeaks(std::array<pixel, 360> pixelOccurences, int dominantCount);
    void enlivenPeaksColors(std::vector<std::pair<pixel, int>> &peaks);

    template <typename Type>
    std::vector<Type> get(const std::string& imagePath, int dominantCount, bool enliven = false) {
        auto pixelOccurences = getPixelsOccurencesFromImagePath(imagePath);
        std::vector<std::pair<pixel, int>> peaks = getPeaks(pixelOccurences, dominantCount);

        if (enliven) {
            enlivenPeaksColors(peaks);
        }

        ColorConverter<Type> colorConverter;

        std::vector<Type> result(peaks.size());
        std::transform(peaks.begin(), peaks.end(), result.begin(), colorConverter);

        return result;
    }

    template <>
    struct ColorConverter<RGB> {
        RGB operator()(const std::pair<pixel, int> &peak) {
            return HSVToRGB({static_cast<double>(peak.second), peak.first.sAverage, peak.first.vAverage});
        }
    };

    template <>
    struct ColorConverter<HSV> {
        HSV operator()(const std::pair<pixel, int> &peak) {
            return {static_cast<double>(peak.second), peak.first.sAverage, peak.first.vAverage};
        }
    };

    template <>
    struct ColorConverter<std::string> {
        std::string operator()(const std::pair<pixel, int> &peak) {
            RGB rgb = HSVToRGB({static_cast<double>(peak.second), peak.first.sAverage, peak.first.vAverage});
            return RGBToHex(rgb);
        }
    };
}
