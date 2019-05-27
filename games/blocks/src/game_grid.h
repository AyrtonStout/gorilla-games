#ifndef HELLO_GAME_GRID_H
#define HELLO_GAME_GRID_H


#include <vector>
#include "block.h"

using namespace std;

class GameGrid {
public:
    GameGrid();
    void swap_panels(int x, int y);
    vector<vector<Block>> blocks;
    static const int GAME_HEIGHT = 12;
    static const int GAME_WIDTH = 6;

private:
    void check_for_matches();
};


#endif
