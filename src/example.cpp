#include "DominantColor.h"

/*
    If you wish to use the C++ library directly, here are the same examples as in the example.js
    file at the root of the project, but written in C++.
*/

int main() {
    std::vector<DominantColor::RGB> dominantColor = DominantColor::get<DominantColor::RGB>("../images/big.jpg", 3, false);

    for (const auto &element : dominantColor) {
        printf("rgb(%d, %d, %d)\n", element.r, element.g, element.b);
    }
}