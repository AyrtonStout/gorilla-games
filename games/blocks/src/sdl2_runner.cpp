#include <SDL_image.h>
#include <thread>
#include "sdl2_runner.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// This is some hacky dev-environment nonsense
#ifndef __EMSCRIPTEN__
const string pathPrefix = "../"; // NOLINT(cert-err58-cpp)
#else
const string pathPrefix = "./";
#endif

#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-pro-type-member-init"


// This is an unfortunate necessity to the fact that the `emscripten_set_main_loop` series of functions
// want a 'pointer-to-function', and class functions are considered 'pointer-to-member-function'.
// So we have to wrap the member function call in a non-member function call
void updateWrapper(void *runner) {
    auto sdl2_runner = (Sdl2Runner*) runner;
    sdl2_runner->update();
}

#pragma clang diagnostic ignored "-Wunknown-pragmas"
Sdl2Runner::Sdl2Runner(GameState *game_state) {
    this->game_state = game_state;

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;

    SDL_CreateWindowAndRenderer(500, 600, 0, &window, &renderer);

    // Set up a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    load_textures();

    // Show what is in the renderer
    SDL_RenderPresent(renderer);
}
#pragma clang diagnostic pop

// TODO need to handle a separate update interval for Emscripten somehow
// Might possibly want to use "emscripten_async_call" for rendering, and run game updates in a new thread?
void Sdl2Runner::run() {
#ifdef __EMSCRIPTEN__
    // If using emscripten, have our main loop be browser friendly
  emscripten_set_main_loop_arg(updateWrapper, this, 0, 1);
#else
    // Otherwise, do a janky normal loop
    while (running) {
        update();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
#endif
}


void Sdl2Runner::load_textures() {
    const string file_path = pathPrefix + "assets/images/";

    SDL_Surface *cursor_image = IMG_Load((file_path + "cursor.png").c_str());
    if (!cursor_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    cursor_texture = SDL_CreateTextureFromSurface(renderer, cursor_image);
    SDL_FreeSurface(cursor_image);

    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second;

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        block_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }
}

void Sdl2Runner::process_keypress(SDL_Event event) {
    auto cursor = &game_state->game_cursor;

    switch (event.key.keysym.sym)
    {
        case SDLK_UP:
            cursor->move(UP); break;
        case SDLK_DOWN:
            cursor->move(DOWN); break;
        case SDLK_LEFT:
            cursor->move(LEFT); break;
        case SDLK_RIGHT:
            cursor->move(RIGHT); break;
        case SDLK_z:
            game_state->game_grid.swap_panels(cursor->get_x(), cursor->get_y()); break;
        default:
            break;
    }
}

void Sdl2Runner::process_input() {

    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_KEYDOWN:
                process_keypress(event);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    running = false;
                }
                break;
        }
    }
}

void Sdl2Runner::update() {
    process_input();

    SDL_RenderClear(renderer);

    for (int y = 0; y < game_state->game_grid.blocks.size(); y++) {
        for (int x = 0; x < game_state->game_grid.blocks[0].size(); x++) {
            SDL_Rect rect = {
                    .x = x * Block::BLOCK_SIZE * SCALING,
                    .y = y * Block::BLOCK_SIZE * SCALING,
                    .w = Block::BLOCK_SIZE * SCALING,
                    .h = Block::BLOCK_SIZE * SCALING
            };

            BlockType block_type = game_state->game_grid.blocks[y][x].get_block_type();

            SDL_RenderCopy(renderer, block_textures[block_type], nullptr, &rect);
        }
    }

    SDL_Rect cursor_rect = {
            .x = game_state->game_cursor.get_x() * Block::BLOCK_SIZE * SCALING - 7,
            .y = game_state->game_cursor.get_y() * Block::BLOCK_SIZE * SCALING - 7,
            .w = GameCursor::CURSOR_WIDTH * SCALING,
            .h = GameCursor::CURSOR_HEIGHT * SCALING
    };

    SDL_RenderCopy(renderer, cursor_texture, nullptr, &cursor_rect);
    SDL_RenderPresent(renderer);

#ifdef __EMSCRIPTEN__
    if (!running) {
        emscripten_cancel_main_loop();
    }
#endif
}

