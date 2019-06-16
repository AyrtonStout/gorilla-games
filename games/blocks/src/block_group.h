#ifndef HELLO_BLOCK_GROUP_H
#define HELLO_BLOCK_GROUP_H


#include <memory>
#include <vector>
#include "block.h"

class BlockGroup {
public:
    vector<shared_ptr<Block>> blocks;
    BlockAction block_action;
};


#endif //HELLO_BLOCK_GROUP_H
