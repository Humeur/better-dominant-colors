var addon = require('bindings')('dominant_color.node')

console.log(addon.getDominantColorsRGB("images/big.jpg", 3, false))