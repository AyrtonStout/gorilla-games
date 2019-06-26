
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include "sdl2_runner.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

GameState state;

#ifdef __EMSCRIPTEN__
EM_JS(void, call_alert, (), {
  globalTestJs();
});
#endif

int main() {
    state = GameState();

    Sdl2Runner(&state.game_grid).run();

#ifdef __EMSCRIPTEN__
    call_alert();
#endif

    return 0;
}


// Expose this function to javascript so that it can update our state
#ifdef __EMSCRIPTEN__

void set_seed(int seed) {
    state.seed = seed;
}

EMSCRIPTEN_BINDINGS(my_module) {
        emscripten::function("set_seed", &set_seed);
}
#endif

