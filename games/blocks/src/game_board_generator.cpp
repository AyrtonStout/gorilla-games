#include <memory>
#include <random>
#include "game_board_generator.h"

vector<vector<shared_ptr<Block>>> GameBoardGenerator::generate_pattern() {
    std::random_device r;
    static std::default_random_engine random(r());

    int pattern_index = random() % generation_patterns.size();
    auto pattern = generation_patterns[pattern_index];

    auto blocks = vector<vector<shared_ptr<Block>>>();

    for (int y = 0; y < pattern.size(); y++) {
        blocks.emplace_back();
        for (int x = 0; x < pattern[0].size(); x++) {
            shared_ptr<Block> block(new Block());
            if (pattern[y][x] == 0) {
                block->deleted = true;
            }
            blocks[y].push_back(block);
        }
    }
    return blocks;
}
