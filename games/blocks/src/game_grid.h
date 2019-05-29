#ifndef HELLO_GAME_GRID_H
#define HELLO_GAME_GRID_H


#include <memory>
#include <unordered_set>
#include <vector>
#include "block.h"

using namespace std;

struct active_block {
    shared_ptr<Block> block;
    int x;
    int y;
};

class GameGrid {
public:
    GameGrid();
    vector<vector<shared_ptr<Block>>> blocks;
    static const int GAME_HEIGHT = 12;
    static const int GAME_WIDTH = 6;

    void swap_panels(int x, int y);
    void update();

private:
    vector<active_block> active_blocks;
    unordered_set<int> active_blocks_ids;

    void check_for_matches();
    void add_active_block(active_block active_block);
    vector<active_block> check_direction(int x, int y, Direction direction);

    void add_new_falling_blocks(vector<active_block> &new_actions, int y, int x);
};


#endif
