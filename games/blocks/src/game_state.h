#ifndef HELLO_GAME_STATE_H
#define HELLO_GAME_STATE_H


#include "game_grid.h"

class GameState {
public:
    GameGrid p1_game_grid;
    GameGrid p2_game_grid;
    int seed;
    int num_players;

    explicit GameState(int num_players);
    void update();
};


#endif //HELLO_GAME_STATE_H
