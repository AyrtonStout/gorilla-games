#include <SDL_image.h>
#include <thread>
#include <iostream>
#include "sdl2_grid_renderer.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

// This is some hacky dev-environment nonsense
#ifndef __EMSCRIPTEN__
const string pathPrefix = "../"; // NOLINT(cert-err58-cpp)
#else
const string pathPrefix = "./";
#endif


Sdl2GridRenderer::Sdl2GridRenderer(sdl_game_textures *textures, GameGrid *game_grid) {
    this->textures = textures;
    this->game_grid = game_grid;
}


int Sdl2GridRenderer::get_draw_point_for_block_height(int coordinate) {
    return (coordinate * Block::BLOCK_SIZE + BACKGROUND_GAME_HEIGHT_OFFSET - game_grid->get_stack_increase_height()) * SCALING;
}

int get_special_indicator_animation_offset(int frames_remaining) {
    int fast_appearance_offset = 7 - GameGrid::SPECIAL_FRAME_START_DURATION + frames_remaining;
    if (fast_appearance_offset < 0) {
        fast_appearance_offset = 0;
    }

    int slow_appearance_offset = 25 - GameGrid::SPECIAL_FRAME_START_DURATION + frames_remaining;
    if (slow_appearance_offset < 0) {
        slow_appearance_offset = 0;
    }

    return fast_appearance_offset + (slow_appearance_offset / 2);
}

void Sdl2GridRenderer::render(SDL_Renderer *renderer) {
    // TODO Background rect shouldn't ever change. might not need to redo this every update but not sure the best way to not do that
    SDL_Rect background_rect = {
            .x = 0,
            .y = 0,
            .w = BACKGROUND_WIDTH * SCALING,
            .h = BACKGROUND_HEIGHT * SCALING
    };

    for (int y = 0; y < game_grid->blocks.size(); y++) {
        for (int x = 0; x < game_grid->blocks[0].size(); x++) {
            auto block = game_grid->blocks[y][x];
            if (!block->is_visible()) {
                continue;
            }

            SDL_Rect rect = {
                    .x = (x * Block::BLOCK_SIZE + block->get_render_offset_x() + BACKGROUND_GAME_WIDTH_OFFSET) * SCALING,
                    .y = ((y + 1) * Block::BLOCK_SIZE + block->get_render_offset_y() + BACKGROUND_GAME_HEIGHT_OFFSET - game_grid->get_stack_increase_height()) * SCALING,
                    .w = Block::BLOCK_SIZE * SCALING,
                    .h = Block::BLOCK_SIZE * SCALING
            };

            BlockType block_type = block->block_type;
            SDL_Texture *texture;
            if (block->block_action == BlockAction::FLASHING_1) {
                int frame = block->get_action_frames_remaining();
                if (frame % 4 == 0 || frame % 4 == 1) {
                    texture = textures->block_flash_textures[block_type];
                } else {
                    texture = textures->block_textures[block_type];
                }
            } else if (block->block_action == BlockAction::FLASHING_2) {
                texture = textures->block_face_textures[block_type];
            } else {
                texture = textures->block_textures[block_type];
            }

            SDL_RenderCopy(renderer, texture, nullptr, &rect);
            if (block->inaccessible) {
                SDL_RenderCopy(renderer, textures->black_tint_texture, nullptr, &rect);
            }
        }
    }

    auto cursor = game_grid->game_cursor;

    SDL_Rect cursor_rect = {
            .x = (cursor.x * Block::BLOCK_SIZE + BACKGROUND_GAME_WIDTH_OFFSET) * SCALING - 7,
            .y = get_draw_point_for_block_height(cursor.y + 1) - 7,
            .w = GameCursor::CURSOR_WIDTH * SCALING,
            .h = GameCursor::CURSOR_HEIGHT * SCALING
    };

    SDL_RenderCopy(renderer, textures->background_texture, nullptr, &background_rect);
    SDL_RenderCopy(renderer, textures->cursor_texture, nullptr, &cursor_rect);

    for (auto indicator : game_grid->get_combo_indications()) {
        int double_panel_offset = 0;
        if (indicator.both_special_triggered && indicator.pop_type == PopType::CHAIN) {
            double_panel_offset = (Block::BLOCK_SIZE + 1) * SCALING;
        }

        // Make the indicator kind of smoothly move up after it appears
        int animation_offset = get_special_indicator_animation_offset(indicator.frames_remaining);

        SDL_Rect rect = {
                .x = (indicator.x * Block::BLOCK_SIZE + BACKGROUND_GAME_WIDTH_OFFSET) * SCALING,
                .y = get_draw_point_for_block_height(indicator.y) - double_panel_offset + animation_offset,
                .w = Block::BLOCK_SIZE * SCALING,
                .h = Block::BLOCK_SIZE * SCALING
        };

        auto texture = indicator.pop_type == PopType::COMBO ? textures->combo_textures[indicator.size] : textures->chain_textures[indicator.size];

        SDL_RenderCopy(renderer, texture, nullptr, &rect);
    }
}

