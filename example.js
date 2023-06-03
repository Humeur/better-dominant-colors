var dominantColorsSmart = require('bindings')('dominant_colors_smart.node')

/*
    Array getDominantColors(String imagePath, Integer dominantCount, Boolean enliven = true);

    Returns an array of strings containing the hexadecimal codes of the dominantCount colors of
    the image given in the imagePath argument.

    The enliven option allows you to increase the saturation and value of the image's HSV values
    by 50% to obtain brighter colors. It is enabled by default.
*/

dominantColorsSmall = dominantColorsSmart.getDominantColors("images/small.png", 3)
dominantColorsMedium = dominantColorsSmart.getDominantColors("images/medium.jpg", 3, false)

/*
    A few different implementations for specific needs :

    - Array getDominantColorsRGB(String imagePath, Integer dominantCount, Boolean enliven = true);
    Returns an array of integers composed of the RGB components of the dominant colors.
*/

dominantColorsBigRGB = dominantColorsSmart.getDominantColorsRGB("images/big.jpg", 4, false)

/*
    - Array getDominantColorsHSV(String imagePath, Integer dominantCount, Boolean enliven = true);
    Returns an array of integers composed of the HSV components of the dominant colors.
*/

dominantColorsBigHSV = dominantColorsSmart.getDominantColorsHSV("images/big.jpg", 4, false)


console.log(dominantColorsSmall, dominantColorsMedium, dominantColorsBigRGB, dominantColorsBigHSV)
/*
    Output of console.log to help you visualize what you get from each functions.

    ['#ff04', '#3c719c', '#950eef']
    ['#1a8ee1', '#699c3c', '#fa5a04']
    [63, 95, 133, 162, 128, 58,  6,   6,   6,  10, 11, 11]
    Float64Array(12) [213,0.5236050823000705,0.5236050823000705, 40, 0.6377422571726704, 0.6377422571726704,
    120, 0.027078878512844205, 0.027078878512844205, 180, 0.04332446878479159, 0.04332446878479159]
*/