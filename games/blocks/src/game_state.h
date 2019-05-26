#ifndef HELLO_GAME_STATE_H
#define HELLO_GAME_STATE_H


#include "game_cursor.h"
#include "game_grid.h"

class GameState {
public:
    GameCursor game_cursor;
    GameGrid game_grid;
    int seed;

    GameState();
};


#endif //HELLO_GAME_STATE_H
