{
    "targets": [{
        "target_name": "MaiaNative",
        "sources": [  '<!@(ls -1 src/je/*.cpp)', '<!@(ls -1 src/*.cpp)', '<!@(ls -1 src/pathing/*.cpp)', "src/pathing/AStar.c" , '<!@(ls -1 src/blocks/*.cpp)', ],
        "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
        "defines": [ "NAPI_DISABLE_CPP_EXCEPTIONS" ]
    }]
}