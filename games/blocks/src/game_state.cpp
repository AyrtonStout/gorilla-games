#include "game_state.h"

GameState::GameState(int num_players) {
    p1_game_grid = GameGrid();
    p2_game_grid = GameGrid();

    this->num_players = num_players;
}

void GameState::update() {
    p1_game_grid.update();

    if (this->num_players == 2) {
        p2_game_grid.update();
    }
}
