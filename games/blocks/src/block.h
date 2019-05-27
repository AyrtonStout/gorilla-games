#ifndef HELLO_BLOCK_H
#define HELLO_BLOCK_H

#include <map>
#include <string>
#include "direction.h"

using namespace std;

enum BlockType { L_BLUE, D_BLUE, RED, GREEN, PURPLE, YELLOW, COUNT };


class Block {

public:
    Block();

    BlockType get_block_type();
    int get_id();
    int get_render_offset_x();
    int get_render_offset_y();
    bool start_move(Direction p_direction);

    bool deleted = false;
    static const int BLOCK_SIZE = 16;

    static const map<BlockType, string> block_to_file_name;

private:
    int render_offset_x;
    int render_offset_y;
    BlockType block_type;
    Direction direction;

    int id;
    static int next_block_id;
};


#endif //HELLO_BLOCK_H
