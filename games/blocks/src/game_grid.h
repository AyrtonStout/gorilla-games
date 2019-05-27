#ifndef HELLO_GAME_GRID_H
#define HELLO_GAME_GRID_H


#include <vector>
#include "block.h"

using namespace std;

struct active_block {
    Block *block;
    BlockAction block_action;
    int x;
    int y;
};

class GameGrid {
public:
    GameGrid();
    vector<vector<Block>> blocks;
    static const int GAME_HEIGHT = 12;
    static const int GAME_WIDTH = 6;

    void swap_panels(int x, int y);
    void update();

private:
    vector<active_block> active_blocks;

    void check_for_matches();
};


#endif
