#include "game_grid.h"

const int GRID_WIDTH = 6;
const int GRID_HEIGHT = 12;

GameGrid::GameGrid() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        blocks.emplace_back();
        for (int x = 0; x < GRID_WIDTH; x++) {
            blocks[y].emplace_back(x, y);
        }
    }
}
