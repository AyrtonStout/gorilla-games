#ifndef HELLO_GAME_GRID_H
#define HELLO_GAME_GRID_H


#include <vector>
#include "block.h"

using namespace std;

class GameGrid {
public:
    GameGrid();
    vector<vector<Block>> blocks;
private:
};


#endif
