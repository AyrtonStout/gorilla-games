
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "sdl2_runner.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

using namespace std;


int main() {
    GameState state = GameState();

    Sdl2Runner(&state).run();

    return 0;
}

