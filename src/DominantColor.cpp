#include "DominantColor.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../lib/stb_image_resize.h"

namespace DominantColor {
bool double_equals(double a, double b, double epsilon) { return std::abs(a - b) < epsilon; }

std::string RGBToHex(const RGB &pixel) {
    std::stringstream ss;
    ss << "#" << std::hex << (pixel.r << 16 | pixel.g << 8 | pixel.b);
    return ss.str();
}

HSV RGBToHSV(const RGB &pixel) {
    auto normalizedR = static_cast<double>(pixel.r / 255.0);
    auto normalizedG = static_cast<double>(pixel.g / 255.0);
    auto normalizedB = static_cast<double>(pixel.b / 255.0);

    double cMax = std::max(normalizedR, std::max(normalizedG, normalizedB));
    double cMin = std::min(normalizedR, std::min(normalizedG, normalizedB));
    double delta = cMax - cMin;

    HSV hsv{};

    if (double_equals(delta, 0.0)) {
        hsv.h = 0.0;
    } else if (double_equals(cMax, normalizedR)) {
        hsv.h = 60.0 * ((normalizedG - normalizedB) / delta);
    } else if (double_equals(cMax, normalizedG)) {
        hsv.h = 60.0 * ((normalizedB - normalizedR) / delta + 2.0);
    } else if (double_equals(cMax, normalizedB)) {
        hsv.h = 60.0 * ((normalizedR - normalizedG) / delta + 4.0);
    }

    if (hsv.h < 0) {
        hsv.h += 360;
    }

    if (double_equals(cMax, 0.0)) {
        hsv.s = 0.0;
    } else {
        hsv.s = delta / cMax;
    }

    hsv.v = cMax;

    return hsv;
}

RGB HSVToRGB(const HSV &pixel) {
    double c = pixel.v * pixel.s;
    double x = c * (1 - std::abs(std::fmod(pixel.h / 60.0, 2) - 1));
    double m = pixel.v - c;

    double normalizedR = 0, normalizedG = 0, normalizedB = 0;
    if (pixel.h < 60) {
        normalizedR = c; normalizedG = x;
    } else if (pixel.h < 120) {
        normalizedR = x; normalizedG = c;
    } else if (pixel.h < 180) {
        normalizedG = c; normalizedB = x;
    } else if (pixel.h < 240) {
        normalizedG = x; normalizedB = c;
    } else if (pixel.h < 300) {
        normalizedR = x; normalizedB = c;
    } else if (pixel.h < 360) {
        normalizedR = c; normalizedB = x;
    }

    return {static_cast<unsigned char>((normalizedR + m) * 255),
            static_cast<unsigned char>((normalizedG + m) * 255),
            static_cast<unsigned char>((normalizedB + m) * 255)
    };
}

void getNewSizeFromMaxSize(int *originalWidth, int *originalHeight) {
    int originalSize = *originalWidth * *originalHeight;

    if (originalSize > MAX_SIZE) {
        double scaleFactor = std::sqrt(static_cast<double>(MAX_SIZE) / originalSize);
        *originalWidth = static_cast<int>(*originalWidth * scaleFactor);
        *originalHeight = static_cast<int>(*originalHeight * scaleFactor);
    }
}

std::unique_ptr<unsigned char[]> resizeImage(std::unique_ptr<unsigned char[]> imageSrc, int *width, int *height) {
    const int originalWidth = *width;
    const int originalHeight = *height;
    getNewSizeFromMaxSize(width, height);

    std::unique_ptr<unsigned char[]> resizedImage(new unsigned char[*width * *height * 3]);

    if (!stbir_resize_uint8(imageSrc.get(), originalWidth, originalHeight, 0, resizedImage.get(), *width, *height, 0, 3)) {
        throw std::runtime_error("resizeImage: failed to resize image");
    }

    return resizedImage;
}


std::array<pixel, 360> getPixelsOccurencesFromImagePath(std::string imagePath) {
    int width, height, channels;
    std::unique_ptr<unsigned char []> imageTmp(stbi_load(imagePath.data(), &width, &height, &channels, STBI_rgb));

    if (!imageTmp) {
        throw std::runtime_error("getPixelsOccurencesFromImagePath: failed to load image from imagePath");
    }

    std::unique_ptr<unsigned char []> image = resizeImage(std::move(imageTmp), &width, &height);

    std::array<pixel, 360> pixelsOccurences{};
    pixelsOccurences.fill({0, 100, 100});

    for (int x = 0 ; x < width ; x++) {
        for (int y = 0 ; y < height ; y++) {
            int pixelIndex = 3 * (y * width + x);
            RGB pixelRGB = {image[pixelIndex], image[pixelIndex + 1], image[pixelIndex + 2]};
            HSV pixelHSV = RGBToHSV(pixelRGB);
            int pixelH = static_cast<int>(pixelHSV.h);

            pixel pixelOccurence = pixelsOccurences[pixelH];

            pixelOccurence.sAverage = (pixelOccurence.count * pixelOccurence.sAverage + pixelHSV.s) / (pixelOccurence.count + 1);
            pixelOccurence.vAverage = (pixelOccurence.count * pixelOccurence.vAverage + pixelHSV.s) / (pixelOccurence.count + 1);
            pixelOccurence.count += 1;

            pixelsOccurences[pixelH] = pixelOccurence;
        }
    }

    return pixelsOccurences;
}

void enlivenPeaksColors(std::vector<std::pair<pixel, int>> &peaks) {
    for (int i = 0 ; i < peaks.size() ; ++i) {
        peaks[i].first.sAverage *= 1.5;
        peaks[i].first.vAverage *= 1.5;
        if (peaks[i].first.sAverage > 1) peaks[i].first.sAverage = 1;
        if (peaks[i].first.vAverage > 1) peaks[i].first.vAverage = 1;
    }
}

std::vector<std::pair<pixel, int>> getPeaks(std::array<pixel, 360> pixelOccurences, int dominantCount) {
    std::vector<float> x;
    std::vector<int> peaks;

    for (auto & pixelOccurence : pixelOccurences) {
        x.emplace_back(pixelOccurence.count);
    }

    PeakFinder::findPeaks(x, peaks, false, 100.0);


    std::vector<std::pair<pixel, int>> peaksToPixel(peaks.size());
    std::transform(peaks.cbegin(), peaks.cend(), peaksToPixel.begin(), [&pixelOccurences](int index) {
        return std::pair<pixel, int>{pixelOccurences[index], index};
    });

    std::sort(peaksToPixel.begin(), peaksToPixel.end(), [](const std::pair<pixel, int> &a, const std::pair<pixel, int> &b) {
        return a.first.count > b.first.count;
    });

    return {peaksToPixel.begin(), peaksToPixel.begin() + dominantCount};
}
}