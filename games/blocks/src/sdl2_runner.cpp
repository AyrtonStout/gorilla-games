#include <SDL_image.h>
#include <thread>
#include <iostream>
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

    SDL_CreateWindowAndRenderer(BACKGROUND_WIDTH * SCALING, BACKGROUND_HEIGHT * SCALING, 0, &window, &renderer);

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

    // Cursor
    SDL_Surface *cursor_image = IMG_Load((file_path + "cursor.png").c_str());
    if (!cursor_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    cursor_texture = SDL_CreateTextureFromSurface(renderer, cursor_image);
    SDL_FreeSurface(cursor_image);


    // Background
    SDL_Surface *background_image = IMG_Load((file_path + "game-background.png").c_str());
    if (!background_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    background_texture = SDL_CreateTextureFromSurface(renderer, background_image);
    SDL_FreeSurface(background_image);


    // Black tint
    SDL_Surface *black_tint_image = IMG_Load((file_path + "black-tint.png").c_str());
    if (!black_tint_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    black_tint_texture = SDL_CreateTextureFromSurface(renderer, black_tint_image);
    SDL_FreeSurface(black_tint_image);


    // Normal block images
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second + ".png";

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        block_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }


    // Flashing block images
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second + "-flash.png";

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        block_flash_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }


    // Face block images
    for (int i = 0; i < BlockType::COUNT; i++) {
        auto block_type = (BlockType) i;
        auto map_entry = Block::block_to_file_name.find(block_type);
        auto file_name = map_entry->second + "-face.png";

        SDL_Surface *image = IMG_Load((file_path + file_name).c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        block_face_textures.emplace(block_type, texture);

        SDL_FreeSurface(image);
    }


    // Combo images
    for (int i = 4; i <= GameGrid::MAX_COMBO; i++) {
        SDL_Surface *image = IMG_Load((file_path + "combo/combo-" + to_string(i) + ".png").c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        combo_textures.emplace(i, texture);

        SDL_FreeSurface(image);
    }

    SDL_Surface *combo_unknown_image = IMG_Load((file_path + "combo/combo-unknown.png").c_str());
    if (!combo_unknown_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    combo_unknown_texture = SDL_CreateTextureFromSurface(renderer, combo_unknown_image);
    SDL_FreeSurface(combo_unknown_image);


    // Chain images
    for (int i = 2; i <= GameGrid::MAX_COMBO; i++) {
        SDL_Surface *image = IMG_Load((file_path + "chain/chain-" + to_string(i) + ".png").c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        chain_textures.emplace(i, texture);

        SDL_FreeSurface(image);
    }

    SDL_Surface *chain_unknown_image = IMG_Load((file_path + "chain/chain-unknown.png").c_str());
    if (!chain_unknown_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    chain_unknown_texture = SDL_CreateTextureFromSurface(renderer, chain_unknown_image);
    SDL_FreeSurface(chain_unknown_image);
}

void Sdl2Runner::process_keypress(SDL_Event event) {
    auto grid = &game_state->game_grid;
    auto cursor = &game_state->game_grid.game_cursor;

    switch (event.key.keysym.sym)
    {
        case SDLK_UP:
            grid->move_cursor(Direction::UP); break;
        case SDLK_DOWN:
            grid->move_cursor(Direction::DOWN); break;
        case SDLK_LEFT:
            grid->move_cursor(Direction::LEFT); break;
        case SDLK_RIGHT:
            grid->move_cursor(Direction::RIGHT); break;
        case SDLK_z:
            game_state->game_grid.swap_panels(cursor->x, cursor->y); break;
        case SDLK_x:
            game_state->game_grid.stack_raise_requested = true;
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
            case SDL_KEYUP:
                switch (event.key.keysym.sym) {
                    case SDLK_x:
                        game_state->game_grid.stack_raise_requested = false;
                        break;
                }
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
    // FIXME This shouldn't be the responsibility of SDL to do this update
    game_state->game_grid.update();

    process_input();

    SDL_RenderClear(renderer);

    // TODO Background rect shouldn't ever change. might not need to redo this every update but not sure the best way to not do that
    SDL_Rect background_rect = {
            .x = 0,
            .y = 0,
            .w = BACKGROUND_WIDTH * SCALING,
            .h = BACKGROUND_HEIGHT * SCALING
    };

    for (int y = 0; y < game_state->game_grid.blocks.size(); y++) {
        for (int x = 0; x < game_state->game_grid.blocks[0].size(); x++) {
            auto block = game_state->game_grid.blocks[y][x];
            if (!block->is_visible()) {
                continue;
            }

            SDL_Rect rect = {
                    .x = (x * Block::BLOCK_SIZE + block->get_render_offset_x() + BACKGROUND_GAME_WIDTH_OFFSET) * SCALING,
                    .y = ((y + 1) * Block::BLOCK_SIZE + block->get_render_offset_y() + BACKGROUND_GAME_HEIGHT_OFFSET - game_state->game_grid.get_stack_increase_height()) * SCALING,
                    .w = Block::BLOCK_SIZE * SCALING,
                    .h = Block::BLOCK_SIZE * SCALING
            };

            BlockType block_type = block->block_type;
            SDL_Texture *texture;
            if (block->block_action == BlockAction::FLASHING_1) {
                int frame = block->get_action_frames_remaining();
                if (frame % 4 == 0 || frame % 4 == 1) {
                    texture = block_flash_textures[block_type];
                } else {
                    texture = block_textures[block_type];
                }
            } else if (block->block_action == BlockAction::FLASHING_2) {
                texture = block_face_textures[block_type];
            } else {
                texture = block_textures[block_type];
            }

            SDL_RenderCopy(renderer, texture, nullptr, &rect);
            if (block->inaccessible) {
                SDL_RenderCopy(renderer, black_tint_texture, nullptr, &rect);
            }
        }
    }

    auto cursor = game_state->game_grid.game_cursor;

    SDL_Rect cursor_rect = {
            .x = (cursor.x * Block::BLOCK_SIZE + BACKGROUND_GAME_WIDTH_OFFSET) * SCALING - 7,
            .y = ((cursor.y + 1) * Block::BLOCK_SIZE + BACKGROUND_GAME_HEIGHT_OFFSET - game_state->game_grid.get_stack_increase_height()) * SCALING - 7,
            .w = GameCursor::CURSOR_WIDTH * SCALING,
            .h = GameCursor::CURSOR_HEIGHT * SCALING
    };

    SDL_RenderCopy(renderer, background_texture, nullptr, &background_rect);
    SDL_RenderCopy(renderer, cursor_texture, nullptr, &cursor_rect);

    for (auto indicator : game_state->game_grid.get_combo_indications()) {
        SDL_Rect rect = {
                .x = (indicator.x * Block::BLOCK_SIZE + BACKGROUND_GAME_WIDTH_OFFSET) * SCALING,
                .y = (indicator.y * Block::BLOCK_SIZE + BACKGROUND_GAME_HEIGHT_OFFSET - game_state->game_grid.get_stack_increase_height()) * SCALING,
                .w = Block::BLOCK_SIZE * SCALING,
                .h = Block::BLOCK_SIZE * SCALING
        };

        auto texture = indicator.pop_type == PopType::COMBO ? combo_textures[indicator.size] : chain_textures[indicator.size];

        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }

    SDL_RenderPresent(renderer);

#ifdef __EMSCRIPTEN__
    if (!running) {
        emscripten_cancel_main_loop();
    }
#endif
}

