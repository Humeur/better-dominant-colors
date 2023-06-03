cmake_minimum_required(VERSION 3.24)
project(dominant_colors_smart)

set(CMAKE_CXX_STANDARD 17)

add_executable(dominant_color src/DominantColor.cpp lib/stb_image.h lib/stb_image_resize.h src/DominantColor.h src/PeakFinder.cpp src/PeakFinder.h src/example.cpp)
