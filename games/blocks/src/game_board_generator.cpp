#include <memory>
#include <random>
#include <iostream>
#include "game_board_generator.h"
#include "game_grid.h"

// When we fill in blocks, we do it top to bottom, left to right. When we generate a random block, we cannot
// place a block next to a block of the same color. Because of the direction that we fill in blocks, this means
// that we need to make sure the block above and block to the left do not match what we generated
BlockType get_valid_block_type(const shared_ptr<Block>& left_block, const shared_ptr<Block>& top_block) {

    // This infinitely generates a block until it eventually grabs one that isn't the same as the block above it or to the left.
    // Could maintain a set of blocks and pull out the ones adjacent to it prior to picking instead of doing the infinite loop;
    // but I actually feel more confident that, on average, this will be more performant. Not that it matters much

    while (true) {
        auto potential_block_type = (BlockType) (random() % BlockType::COUNT);

        if (top_block != nullptr && !top_block->deleted && top_block->block_type == potential_block_type) {
            continue;
        }

        if (left_block != nullptr && !left_block->deleted && left_block->block_type == potential_block_type) {
            continue;
        }

        return potential_block_type;
    }
}

vector<vector<shared_ptr<Block>>> GameBoardGenerator::generate_pattern() {
    std::random_device r;
    static std::default_random_engine random(r());

    int pattern_index = random() % generation_patterns.size();
    auto pattern = generation_patterns[pattern_index];

    auto blocks = vector<vector<shared_ptr<Block>>>();

    for (int y = 0; y < GameGrid::GAME_HEIGHT; y++) {
        blocks.emplace_back();
        for (int x = 0; x < GameGrid::GAME_WIDTH; x++) {
            shared_ptr<Block> block(new Block());
            if (pattern[y][x] == 0) {
                block->deleted = true;
            } else {
                auto top_block = y > 0 ? blocks[y - 1][x] : nullptr;
                auto left_block = x > 0 ? blocks[y][x - 1] : nullptr;
                block->block_type = get_valid_block_type(left_block, top_block);
            }
            blocks[y].push_back(block);
        }
    }
    return blocks;
}

// TODO have this remember state of the last row so it doesn't generate with like blocks touching
vector<shared_ptr<Block>> GameBoardGenerator::generate_row() {
    auto blocks = vector<shared_ptr<Block>>();

    for (int x = 0; x < GameGrid::GAME_WIDTH; x++) {
        shared_ptr<Block> block(new Block());
        auto left_block = x > 0 ? blocks[x - 1] : nullptr;

        block->block_type = get_valid_block_type(left_block, nullptr);
        blocks.push_back(block);
    }

    return blocks;
}
