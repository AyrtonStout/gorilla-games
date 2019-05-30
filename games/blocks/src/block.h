#ifndef HELLO_BLOCK_H
#define HELLO_BLOCK_H

#include <map>
#include <string>
#include "direction.h"

using namespace std;

// As much as I would love to use a more robust Enum Class here, it makes random access more cumbersome
enum BlockType { L_BLUE, D_BLUE, RED, GREEN, PURPLE, YELLOW, COUNT };

enum class BlockAction { NONE, SLIDE_LEFT, SLIDE_RIGHT, FALLING, SLIDE_FLOAT, POP_FLOAT, FLOATING, FLASHING_1, FLASHING_2 };

class Block {

public:
    Block();

    BlockAction block_action;
    BlockType block_type;
    int get_id();
    int get_render_offset_x();
    int get_render_offset_y();
    int get_action_frames_remaining();
    bool is_action_done();
    void transition_to_state(BlockAction action);
    void complete_action();
    void update();
    bool can_be_matched_with();
    bool can_prevent_falling();

    bool deleted = false;
    static const int BLOCK_SIZE = 16;

    static const map<BlockType, string> block_to_file_name;

private:
    int render_offset_x;
    int render_offset_y;

    int id;
    int action_frames_remaining;
    bool action_done;
    static int next_block_id;
};


#endif //HELLO_BLOCK_H
