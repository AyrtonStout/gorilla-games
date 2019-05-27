#include <iostream>
#include <unordered_set>
#include "direction.h"
#include "game_grid.h"


GameGrid::GameGrid() {
    for (int y = 0; y < GameGrid::GAME_HEIGHT; y++) {
        blocks.emplace_back();
        for (int x = 0; x < GameGrid::GAME_WIDTH; x++) {
            blocks[y].emplace_back();
        }
    }
}

void GameGrid::swap_panels(int x, int y) {
    swap(blocks[y][x], blocks[y][x + 1]);

    check_for_matches();
}

struct popping_block {
    Block *block;
    int x;
    int y;
};

vector<popping_block> check_direction(vector<vector<Block>> &blocks, int x, int y, Direction direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case UP: dy = -1; break;
        case LEFT: dx = -1; break;
        case RIGHT: dx = 1; break;
        case DOWN: dy = -1; break;
        default: throw std::invalid_argument("Unsupported direction");
    }

    auto current_block = &blocks[y][x];
    if (current_block->deleted) {
        return vector<popping_block>();
    }

    auto found_blocks = vector<popping_block>();
    found_blocks.push_back(popping_block { .block = current_block, .x = x, .y = y });

    while (true) {
        x += dx;
        y += dy;

        if (x < 0 || y < 0 || y > blocks.size() - 1 || x > blocks[0].size() - 1) {
            break;
        }

        auto compare_block = &blocks[y][x];
        if (compare_block->deleted) {
            break;
        }

        if (compare_block->get_block_type() == current_block->get_block_type()) {
            found_blocks.push_back(popping_block { .block = compare_block, .x = x, .y = y });
        } else {
            break;
        }
    }

    if (found_blocks.size() > 2) {
        return found_blocks;
    } else {
        return vector<popping_block>();
    }
}

void GameGrid::check_for_matches() {
    auto popping_block_ids = unordered_set<int>();
    auto popping_blocks = vector<popping_block>();

    for (int y = 0; y < blocks.size(); y++) {
        for (int x = 0; x < blocks[0].size(); x++) {
            Direction directions[] = { UP, LEFT, DOWN, RIGHT };

            for (auto direction : directions) {
                auto found_blocks = check_direction(blocks, x, y, direction);
                for (auto popping_block : found_blocks) {
                    int block_id = popping_block.block->get_id();
                    if (popping_block_ids.count(block_id) == 0) {
                        popping_block_ids.insert(block_id);
                        popping_blocks.push_back(popping_block);
                    }
                }
            }
        }
    }

    for (auto popping_block : popping_blocks) {
        blocks[popping_block.y][popping_block.x].deleted = true;
    }
}
