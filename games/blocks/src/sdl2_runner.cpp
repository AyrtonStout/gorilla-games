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

#pragma clang diagnostic ignored "-Wunknown-pragmas"
Sdl2Runner::Sdl2Runner(GameState *game_state) {
    this->game_state = game_state;

    this->p1_renderer = new Sdl2GridRenderer(&this->textures, &game_state->p1_game_grid, game_state->num_players, 1);
    if (game_state->num_players == 2) {
        this->p2_renderer = new Sdl2GridRenderer(&this->textures, &game_state->p2_game_grid, game_state->num_players, 2);
    }

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window;

    SDL_CreateWindowAndRenderer(FULL_GAME_WIDTH * SCALING, FULL_GAME_HEIGHT * SCALING, 0, &window, &renderer);

    // Set up a white background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    load_textures();

    // Show what is in the renderer
    SDL_RenderPresent(renderer);
}


void Sdl2Runner::load_textures() {
    const string file_path = pathPrefix + "assets/images/";

    // Cursor
    SDL_Surface *cursor_image = IMG_Load((file_path + "cursor.png").c_str());
    if (!cursor_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.cursor_texture = SDL_CreateTextureFromSurface(renderer, cursor_image);
    SDL_FreeSurface(cursor_image);


    // Background
    SDL_Surface *background_image = IMG_Load((file_path + "game-background.png").c_str());
    if (!background_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.background_texture = SDL_CreateTextureFromSurface(renderer, background_image);
    SDL_FreeSurface(background_image);


    // Background 2 Player
    SDL_Surface *background_image_2player = IMG_Load((file_path + "game-background-2p.png").c_str());
    if (!background_image_2player) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.background_texture_2player = SDL_CreateTextureFromSurface(renderer, background_image_2player);
    SDL_FreeSurface(background_image_2player);


    // Black tint
    SDL_Surface *black_tint_image = IMG_Load((file_path + "black-tint.png").c_str());
    if (!black_tint_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.black_tint_texture = SDL_CreateTextureFromSurface(renderer, black_tint_image);
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
        textures.block_textures.emplace(block_type, texture);

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
        textures.block_flash_textures.emplace(block_type, texture);

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
        textures.block_face_textures.emplace(block_type, texture);

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
        textures.combo_textures.emplace(i, texture);

        SDL_FreeSurface(image);
    }

    SDL_Surface *combo_unknown_image = IMG_Load((file_path + "combo/combo-unknown.png").c_str());
    if (!combo_unknown_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.combo_unknown_texture = SDL_CreateTextureFromSurface(renderer, combo_unknown_image);
    SDL_FreeSurface(combo_unknown_image);


    // Chain images
    for (int i = 2; i <= GameGrid::MAX_COMBO; i++) {
        SDL_Surface *image = IMG_Load((file_path + "chain/chain-" + to_string(i) + ".png").c_str());
        if (!image) {
            printf("IMG_Load: %s\n", IMG_GetError());
            continue;
        }
        auto texture = SDL_CreateTextureFromSurface(renderer, image);
        textures.chain_textures.emplace(i, texture);

        SDL_FreeSurface(image);
    }

    SDL_Surface *chain_unknown_image = IMG_Load((file_path + "chain/chain-unknown.png").c_str());
    if (!chain_unknown_image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        return;
    }

    textures.chain_unknown_texture = SDL_CreateTextureFromSurface(renderer, chain_unknown_image);
    SDL_FreeSurface(chain_unknown_image);
}

void Sdl2Runner::process_keypress(SDL_Event event) {
    auto p1_cursor = &game_state->p1_game_grid.game_cursor;
    auto p2_cursor = &game_state->p2_game_grid.game_cursor;
    auto p1_game_grid = &game_state->p1_game_grid;
    auto p2_game_grid = &game_state->p2_game_grid;

    switch (event.key.keysym.sym) {
        case SDLK_UP:
            p1_game_grid->move_cursor(Direction::UP); break;
        case SDLK_DOWN:
            p1_game_grid->move_cursor(Direction::DOWN); break;
        case SDLK_LEFT:
            p1_game_grid->move_cursor(Direction::LEFT); break;
        case SDLK_RIGHT:
            p1_game_grid->move_cursor(Direction::RIGHT); break;
        case SDLK_z:
            p1_game_grid->swap_panels(p1_cursor->x, p1_cursor->y); break;
        case SDLK_x:
            p1_game_grid->stack_raise_requested = true;
        default:
            break;
    }

#ifdef __EMSCRIPTEN__
    auto code = std::to_string(event.key.keysym.sym);
    auto jsFunction = ("sendGameCode(" + code + ")");
    emscripten_run_script(jsFunction.c_str());
#endif

    if (game_state->num_players == 1) {
        return;
    }

    switch (event.key.keysym.sym) {
        case SDLK_KP_8:
            p2_game_grid->move_cursor(Direction::UP); break;
        case SDLK_KP_5:
            p2_game_grid->move_cursor(Direction::DOWN); break;
        case SDLK_KP_4:
            p2_game_grid->move_cursor(Direction::LEFT); break;
        case SDLK_KP_6:
            p2_game_grid->move_cursor(Direction::RIGHT); break;
        case SDLK_KP_0:
            p2_game_grid->swap_panels(p2_cursor->x, p2_cursor->y); break;
        case SDLK_KP_ENTER:
            p2_game_grid->stack_raise_requested = true;
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
                        game_state->p1_game_grid.stack_raise_requested = false;
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

bool Sdl2Runner::update() {
    process_input();

    SDL_RenderClear(renderer);

    // TODO Background rect shouldn't ever change. might not need to redo this every update but not sure the best way to not do that
    SDL_Rect background_rect = {
            .x = 0,
            .y = 0,
            .w = FULL_GAME_WIDTH * SCALING,
            .h = FULL_GAME_HEIGHT * SCALING
    };

    int single_player_start_width = (FULL_GAME_WIDTH / 2) - (Sdl2GridRenderer::GRID_WIDTH / 2);
    int multiplayer_start_width = (FULL_GAME_WIDTH / 2) - Sdl2GridRenderer::GRID_WIDTH - (MULTIPLAYER_GRID_SPACING / 2);
    int p1_render_offset = this->game_state->num_players == 1 ? single_player_start_width : multiplayer_start_width;

    p1_renderer->render(renderer, p1_render_offset, SCALING);

    if (game_state->num_players == 2) {
        p2_renderer->render(renderer, (FULL_GAME_WIDTH / 2) + (MULTIPLAYER_GRID_SPACING / 2), SCALING);
    }

    if (game_state->num_players == 1) {
        SDL_RenderCopy(renderer, textures.background_texture, nullptr, &background_rect);
    } else {
        SDL_RenderCopy(renderer, textures.background_texture_2player, nullptr, &background_rect);
    }

    SDL_RenderPresent(renderer);

    return running;
}

