#include <iostream>
#include <unordered_set>
#include "direction.h"
#include "game_grid.h"

GameGrid::GameGrid() {
    for (int y = 0; y < GameGrid::GAME_HEIGHT; y++) {
        blocks.emplace_back();
        for (int x = 0; x < GameGrid::GAME_WIDTH; x++) {
            shared_ptr<Block> ptr(new Block());
            blocks[y].push_back(ptr);
        }
    }
}

void GameGrid::swap_panels(int x, int y) {
    auto left_block = blocks[y][x];
    auto right_block = blocks[y][x + 1];

    if (left_block->block_action != BlockAction::NONE || right_block->block_action != BlockAction::NONE) {
        return;
    }

    left_block->transition_to_state(BlockAction::SLIDE_RIGHT);
    right_block->transition_to_state(BlockAction::SLIDE_LEFT);

    active_block left_active_block = { .block = left_block, .block_action = BlockAction::SLIDE_RIGHT, .x = x, .y = y };
    active_block right_active_block = { .block = right_block, .block_action = BlockAction::SLIDE_LEFT, .x = x + 1, .y = y };

    active_blocks.push_back(left_active_block);
    active_blocks.push_back(right_active_block);

    check_for_matches();
}

vector<active_block> GameGrid::check_direction(int x, int y, Direction direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case Direction::UP: dy = -1; break;
        case Direction::LEFT: dx = -1; break;
        case Direction::RIGHT: dx = 1; break;
        case Direction::DOWN: dy = -1; break;
        default: throw std::invalid_argument("Unsupported direction");
    }

    auto current_block = blocks[y][x];
    if (current_block->deleted) {
        return vector<active_block>();
    }

    auto found_blocks = vector<active_block>();
    found_blocks.push_back(active_block { .block = current_block, .x = x, .y = y });

    while (true) {
        x += dx;
        y += dy;

        if (x < 0 || y < 0 || y > blocks.size() - 1 || x > blocks[0].size() - 1) {
            break;
        }

        auto compare_block = blocks[y][x];
        if (compare_block->deleted) {
            break;
        }

        if (compare_block->get_block_type() == current_block->get_block_type()) {
            found_blocks.push_back(active_block { .block = compare_block, .x = x, .y = y });
        } else {
            break;
        }
    }

    if (found_blocks.size() > 2) {
        return found_blocks;
    } else {
        return vector<active_block>();
    }
}

void GameGrid::check_for_matches() {
    auto popping_block_ids = unordered_set<int>();
    auto popping_blocks = vector<active_block>();

    for (int y = 0; y < blocks.size(); y++) {
        for (int x = 0; x < blocks[0].size(); x++) {
            Direction directions[] = { Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT };

            for (auto direction : directions) {
                auto found_blocks = check_direction(x, y, direction);
                for (const auto& popping_block : found_blocks) {
                    int block_id = popping_block.block->get_id();
                    if (popping_block_ids.count(block_id) == 0) {
                        popping_block_ids.insert(block_id);
                        popping_blocks.push_back(popping_block);
                    }
                }
            }
        }
    }

    for (const auto& popping_block : popping_blocks) {
        popping_block.block->transition_to_state(BlockAction::FLASHING_1);
        active_blocks.push_back(popping_block);
    }
}

void GameGrid::update() {
    bool match_check_needed = false;

    for (int i = 0; i < active_blocks.size(); i++) {

        int x = active_blocks[i].x;
        int y = active_blocks[i].y;
        active_blocks[i].block->update();

        if (active_blocks[i].block->block_action == BlockAction::NONE) {

            // The block finished sliding to its new location. Swap it with the block next to it
            // Don't check for SLIDE_LEFT, as we only need to do the swap for one completed block
            // Empty blocks still count as blocks, so there will always be a SLIDE_LEFT and SLIDE_RIGHT
            if (active_blocks[i].block_action == BlockAction::SLIDE_RIGHT) {
                swap(blocks[y][x], blocks[y][x + 1]);
                match_check_needed = true;
            }

            active_blocks.erase(active_blocks.begin() + i); // Delete both this and the next block...
            i--;
        }
    }

    if (match_check_needed) {
        check_for_matches();
    }
}
