#include <iostream>
#include "game_grid.h"


GameGrid::GameGrid() {
    for (int y = 0; y < GameGrid::GAME_HEIGHT; y++) {
        blocks.emplace_back();
        for (int x = 0; x < GameGrid::GAME_WIDTH; x++) {
            blocks[y].emplace_back();
        }
    }
}

void GameGrid::swap_panels(int x, int y) {
    swap(blocks[y][x], blocks[y][x + 1]);
}
