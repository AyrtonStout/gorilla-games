#ifndef HELLO_GAME_CURSOR_H
#define HELLO_GAME_CURSOR_H

#include "direction.h"

class GameCursor {
public:
    int y;
    int x;
    bool move(Direction direction, bool allow_bottom_square);
    GameCursor();
    static const int CURSOR_WIDTH = 36;
    static const int CURSOR_HEIGHT = 20;
};


#endif //HELLO_GAME_CURSOR_H
