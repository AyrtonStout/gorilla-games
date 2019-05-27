#ifndef HELLO_BLOCK_H
#define HELLO_BLOCK_H

#include <map>
#include <string>
#include "direction.h"

using namespace std;

enum BlockType { L_BLUE, D_BLUE, RED, GREEN, PURPLE, YELLOW, COUNT };

enum class BlockAction { NONE, SLIDE_LEFT, SLIDE_RIGHT, FALLING, FLASHING_1, FLASHING_2, YELLOW, COUNT };

class Block {

public:
    Block();

    BlockType get_block_type();
    BlockAction block_action;
    int get_id();
    int get_render_offset_x();
    int get_render_offset_y();
    int get_action_frames_remaining();
    void transition_to_state(BlockAction action);
    void update();

    bool deleted = false;
    static const int BLOCK_SIZE = 16;

    static const map<BlockType, string> block_to_file_name;

private:
    int render_offset_x;
    int render_offset_y;
    BlockType block_type;

    int id;
    int action_frames_remaining;
    static int next_block_id;
};


#endif //HELLO_BLOCK_H
