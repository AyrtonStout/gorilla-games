#!/usr/bin/env bash
mkdir -p dist
FILES=$(ls src/*.cpp)
ARGS="-O2 -std=c++11 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s MODULARIZE=1 --preload-file assets -o dist/blocks.js"
echo BUILDING emcc ${FILES} ${ARGS} -s SDL2_IMAGE_FORMATS='["png"]'
emcc ${FILES} ${ARGS} -s SDL2_IMAGE_FORMATS='["png"]'
