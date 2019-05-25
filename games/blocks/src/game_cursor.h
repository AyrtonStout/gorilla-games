#ifndef HELLO_GAME_CURSOR_H
#define HELLO_GAME_CURSOR_H

#include "direction.h"

class GameCursor {
public:
    int get_x();
    int get_y();
    bool move(Direction direction);
    GameCursor();
    static const int CURSOR_WIDTH = 36;
    static const int CURSOR_HEIGHT = 20;

private:
    int m_x;
    int m_y;
};


#endif //HELLO_GAME_CURSOR_H
