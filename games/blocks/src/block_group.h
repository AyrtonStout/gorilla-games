#ifndef HELLO_BLOCK_GROUP_H
#define HELLO_BLOCK_GROUP_H


#include <memory>
#include <vector>
#include "block.h"

class BlockGroup {
    vector<shared_ptr<Block>> blocks;
};


#endif //HELLO_BLOCK_GROUP_H
