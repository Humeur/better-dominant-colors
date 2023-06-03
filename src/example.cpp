#include "DominantColor.h"

/*
    If you wish to use the C++ library directly, here are the same examples as in the example.js
    file at the root of the project, but written in C++.
*/

int main() {
    /*
        Array getDominantColors(String imagePath, Integer dominantCount, Boolean enliven = true);

        Returns an array of strings containing the hexadecimal codes of the dominantCount colors of
        the image given in the imagePath argument.

        The enliven option allows you to increase the saturation and value of the image's HSV values
        by 50% to obtain brighter colors. It is enabled by default.
    */

    std::vector<DominantColor::HexColor> dominantColorsSmall = DominantColor::get<DominantColor::HexColor>("../images/small.png", 3);
    std::vector<DominantColor::HexColor> dominantColorsMedium = DominantColor::get<DominantColor::HexColor>("../images/medium.jpg", 3, false);

    /*
        A few different implementations for specific needs :

        - Array getDominantColorsRGB(String imagePath, Integer dominantCount, Boolean enliven = true);
        Returns an array of integers composed of the RGB components of the dominant colors.
    */

    std::vector<DominantColor::RGB> dominantColorsBigRGB = DominantColor::get<DominantColor::RGB>("../images/big.jpg", 4, false);

    /*
        - Array getDominantColorsHSV(String imagePath, Integer dominantCount, Boolean enliven = true);
        Returns an array of integers composed of the HSV components of the dominant colors.
    */

    std::vector<DominantColor::HSV> dominantColorsBigHSV = DominantColor::get<DominantColor::HSV>("../images/big.jpg", 4, false);


    for (const auto &color : dominantColorsSmall) printf("%s ", color.c_str());
    std::cout << std::endl;
    for (const auto &color : dominantColorsMedium) printf("%s ", color.c_str());
    std::cout << std::endl;
    for (const auto &color : dominantColorsBigRGB) printf("(%d,%d,%d) ", color.r, color.g, color.b);
    std::cout << std::endl;
    for (const auto &color : dominantColorsBigHSV) printf("(%f,%f,%f) ", color.h, color.s, color.v);
    /*
        Output of console.log to help you visualize what you get from each functions.

        #2bc72d #2dc446 #3d5568
        #1a8ee1 #699c3c #6fa33a
        (63,95,133) (162,128,58) (141,117,62) (62,62,46)
        (213.000000,0.523605,0.523605) (40.000000,0.637742,0.637742) (42.000000,0.555068,0.555068) (60.000000,0.243292,0.243292)
    */
}