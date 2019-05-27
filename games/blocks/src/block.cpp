#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <cstdlib>
#include <random>
#include <iostream>
#include "block.h"
#include "frame_timing.h"

std::random_device r;
static std::default_random_engine generator(r());

int Block::next_block_id; // Required to 0-initialize the non-const static variable

const map<BlockType, string> Block::block_to_file_name = {
        { L_BLUE, "lblue-block" },
        { D_BLUE, "dblue-block" },
        { YELLOW, "yellow-block" },
        { GREEN, "green-block" },
        { PURPLE, "purple-block" },
        { RED, "red-block" }
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

Block::Block() {
    this->render_offset_x = 0;
    this->render_offset_y = 0;
    this->block_type = (BlockType) (generator() % BlockType::COUNT);

    id = ++Block::next_block_id;
    block_action = BlockAction::NONE;
    action_frames_remaining = 0;
}

int Block::get_id() {
    return id;
}

void Block::transition_to_state(BlockAction action) {
    block_action = action;
    if (action == BlockAction::SLIDE_LEFT || action == BlockAction::SLIDE_RIGHT) {
        action_frames_remaining = FRAMES_TO_SLIDE;
    } else if (action == BlockAction::FLASHING_1) {
        action_frames_remaining = FRAMES_OF_LIGHT;
    }
}

void Block::update() {
    switch (block_action) {
        case BlockAction::NONE: {
            return;
        }
        case BlockAction::SLIDE_LEFT:
        case BlockAction::SLIDE_RIGHT: {
            if (action_frames_remaining == 0) {
                block_action = BlockAction::NONE;
                render_offset_x = 0;
                return;
            }

            int distance = Block::BLOCK_SIZE / FRAMES_TO_SLIDE;
            if (block_action == BlockAction::SLIDE_LEFT) {
                render_offset_x -= distance;
            } else {
                render_offset_x += distance;
            }
        }
    }

    if (block_action != BlockAction::NONE) {
        action_frames_remaining--;
    }
}

#pragma clang diagnostic pop
