{
  "targets": [
    {
      "target_name": "dominant_colors_smart",
      "cflags!": [ "-fno-exceptions -std=c++17" ],
      "cflags_cc!": [ "-fno-exceptions" ],
      'defines': [
        'NAPI_CPP_EXCEPTIONS',
       ],
      "sources": [
        "./src/DominantColor.cpp",
        "./src/Node.cpp",
        "./src/PeakFinder.cpp",
      ],
      'include_dirs': ["<!(node -p \"require('node-addon-api').include_dir\")"],
    }
  ]
}