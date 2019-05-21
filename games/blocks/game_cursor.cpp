#include <iostream>
#include "game_cursor.h"

using namespace std;

GameCursor::GameCursor() {
    m_x = 4;
    m_y = 0;
}

int GameCursor::get_x() {
    return m_x;
}

int GameCursor::get_y() {
    return m_y;
}

const int GAME_HEIGHT = 12;
const int GAME_WIDTH = 6;

bool GameCursor::move(Direction direction) {
    switch (direction) {
        case UP:
            if (m_y - 1 >= 0) {
                m_y--;
            }
            break;
        case DOWN:
            if (m_y + 1 < GAME_HEIGHT) {
                m_y++;
            }
            break;
        case LEFT:
            if (m_x - 1 >= 0) {
                m_x--;
            }
            break;
        case RIGHT:
            // Cursor is 2 blocks wide, so need a -1 here to account for it
            if (m_x + 1 < GAME_WIDTH - 1) {
                m_x++;
            }
            break;
        default:
            break;
    }
    return false;
}
