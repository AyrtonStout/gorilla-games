
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <thread>
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

struct update_group {
    GameState *state;
    Sdl2Runner *runner;
};

bool running = true;

// This is an unfortunate necessity to the fact that the `emscripten_set_main_loop` series of functions
// want a 'pointer-to-function', and class functions are considered 'pointer-to-member-function'.
// So we have to wrap the member function call in a non-member function call
void updateWrapper(void *runner) {
    auto updates = (update_group*) runner;
    updates->state->update();
    running = updates->runner->update();

#ifdef __EMSCRIPTEN__
    if (!running) {
        emscripten_cancel_main_loop();
    }
#endif
}


int main() {

    state = GameState();
    auto runner = Sdl2Runner(&state);

    update_group updates = { .state = &state, .runner = &runner };

#ifdef __EMSCRIPTEN__
    // If using emscripten, have our main loop be browser friendly
    emscripten_set_main_loop_arg(updateWrapper, &updates, 0, 1);
#else
    // Otherwise, do a janky normal loop
    while (running) {
        updateWrapper(&updates);
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
#endif


#ifdef __EMSCRIPTEN__
    call_alert();
#endif

    return 0;
}

// TODO need to handle a separate update interval for Emscripten somehow
// Might possibly want to use "emscripten_async_call" for rendering, and run game updates in a new thread?

// Expose this function to javascript so that it can update our state
#ifdef __EMSCRIPTEN__

void set_seed(int seed) {
    state.seed = seed;
}

EMSCRIPTEN_BINDINGS(my_module) {
        emscripten::function("set_seed", &set_seed);
}
#endif

