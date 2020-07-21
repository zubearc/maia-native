{
    "targets": [{
        "target_name": "MaiaNative",
        "sources": [ "src/*.cpp", "src/je/*.cpp", "src/pathing/*.c" ],
        "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
        "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }]
}