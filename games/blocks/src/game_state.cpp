#include "game_state.h"

GameState::GameState() {
    game_grid = GameGrid();
}

void GameState::update() {
    game_grid.update();
}
