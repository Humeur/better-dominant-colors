#include "DominantColor.h"

//example using c++ functions...
int main() {
    std::vector<DominantColor::RGB> dominantColor = DominantColor::get<DominantColor::RGB>("../images/big.jpg", 3, false);

    for (const auto &element : dominantColor) {
        printf("rgb(%d, %d, %d)\n", element.r, element.g, element.b);
    }
}