#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <cstdlib>
#include <random>
#include "block.h"

std::random_device r;
static std::default_random_engine generator(r());

int Block::next_block_id; // Required to 0-initialize the non-const static variable

const map<BlockType, string> Block::block_to_file_name = {
        { L_BLUE, "lblue-block.png" },
        { D_BLUE, "dblue-block.png" },
        { YELLOW, "yellow-block.png" },
        { GREEN, "green-block.png" },
        { PURPLE, "purple-block.png" },
        { RED, "red-block.png" }
};

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

Block::Block() {
    this->render_offset_x = 0;
    this->render_offset_y = 0;
    this->direction = NONE;
    this->block_type = (BlockType) (generator() % BlockType::COUNT);

    id = ++Block::next_block_id;
}

int Block::get_id() {
    return id;
}

#pragma clang diagnostic pop
