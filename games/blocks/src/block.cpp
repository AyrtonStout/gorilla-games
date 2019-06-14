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

int Block::get_render_offset_x() {
    return render_offset_x;
}

int Block::get_render_offset_y() {
    return render_offset_y;
}

Block::Block() {
    this->render_offset_x = 0;
    this->render_offset_y = 0;
    this->block_type = L_BLUE; // Doesn't matter the value. It'll be getting changed when the block is made visible

    id = ++Block::next_block_id;
    block_action = BlockAction::NONE;
    action_frames_remaining = 0;
    action_done = false;
}

int Block::get_id() {
    return id;
}

void Block::transition_to_state(BlockAction action) {
    block_action = action;
    action_done = false;
    if (action == BlockAction::SLIDE_LEFT || action == BlockAction::SLIDE_RIGHT) {
        action_frames_remaining = FRAMES_TO_SLIDE;
    } else if (action == BlockAction::FLASHING_1) {
        action_frames_remaining = FRAMES_OF_LIGHT;
    } else if (action == BlockAction::FLASHING_2) {
        action_frames_remaining = FRAMES_OF_FACE;
    } else if (action == BlockAction::SLIDE_FLOAT) {
        block_action = BlockAction::FLOATING;
        action_frames_remaining = FRAMES_TO_START_FALLING;
    } else if (action == BlockAction::POP_FLOAT) {
        block_action = BlockAction::FLOATING;
        action_frames_remaining = FRAME_POP_DROP_DELAY;
    } else if (action == BlockAction::FALLING) {
        action_frames_remaining = FRAMES_TO_FALL;
    }
}

void Block::update() {
    if (block_action != BlockAction::NONE) {
        action_frames_remaining--;
    }

    if (action_frames_remaining == 0) {
        action_done = true;
    }

    switch (block_action) {
        case BlockAction::SLIDE_LEFT:
        case BlockAction::SLIDE_RIGHT: {
            if (action_done) {
                render_offset_x = 0;
                return;
            }

            int distance = Block::BLOCK_SIZE / FRAMES_TO_SLIDE;
            if (block_action == BlockAction::SLIDE_LEFT) {
                render_offset_x -= distance;
            } else {
                render_offset_x += distance;
            }

            break;
        }
        case BlockAction::FLASHING_1: {
            if (action_done) {
                transition_to_state(BlockAction::FLASHING_2);
            }
            return;
        }
        case BlockAction::FLASHING_2: {
            if (action_done) {
                deleted = true;
            }
            return;
        }
        default: {
            return;
        }
    }
}

int Block::get_action_frames_remaining() {
    return action_frames_remaining;
}

bool Block::can_be_matched_with() {
    if (deleted) {
        return false;
    }

    switch (block_action) {
        case BlockAction::NONE:
        case BlockAction::FLOATING:
            return true;
        case BlockAction::FLASHING_1:
            // This means the block was JUST told to start popping this frame. So it's still "matchable" to other blocks around it
            return action_frames_remaining == FRAMES_OF_LIGHT;
        default:
            return false;
    }
}

bool Block::can_prevent_falling() {
    return !deleted && block_action != BlockAction::FALLING;
}

void Block::complete_action() {
    // A block might chain multiple actions together (like falling multiple spots) so this might not always be 0
    if (action_frames_remaining == 0) {
        block_action = BlockAction::NONE;
    }
    action_done = false;
}

bool Block::is_action_done() {
    return action_done;
}

#pragma clang diagnostic pop
