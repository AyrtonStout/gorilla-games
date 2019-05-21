emcc hello_owl.cpp game_cursor.cpp block.cpp game_grid.cpp -O2 -std=c++11 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file assets -o hello_owl.html
