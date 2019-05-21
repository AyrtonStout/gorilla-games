#include <cstdlib>
#include <random>
#include "block.h"

std::random_device r;
static std::default_random_engine generator(r());

const map<BlockType, string> Block::block_to_file_name = {
        { L_BLUE, "lblue-block.png" },
        { D_BLUE, "dblue-block.png" },
        { YELLOW, "yellow-block.png" },
        { GREEN, "green-block.png" },
        { PURPLE, "purple-block.png" },
        { RED, "red-block.png" }
};

int Block::get_grid_x() {
    return grid_x;
}

int Block::get_grid_y() {
    return grid_y;
}

BlockType Block::get_block_type() {
    return block_type;
}

int Block::get_render_offset_x() {
    return render_offset_x;
}

int Block::get_render_offset_y() {
    return render_offset_y;
}

bool Block::start_move(Direction p_direction) {
    this->direction = p_direction;
    return false;
}

Block::Block(int grid_x, int grid_y) {
    this->grid_x = grid_x;
    this->grid_y = grid_y;
    this->render_offset_x = 0;
    this->render_offset_y = 0;
    this->direction = NONE;
    this->block_type = (BlockType) (generator() % BlockType::COUNT);
}
