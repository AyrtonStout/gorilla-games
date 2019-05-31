#include <iostream>
#include "game_cursor.h"
#include "game_grid.h"

using namespace std;

GameCursor::GameCursor() {
    x = 2;
    y = 6;
}

bool GameCursor::move(Direction direction, bool allow_bottom_square) {
    switch (direction) {
        case Direction::UP: {
            if (y - 1 >= 0) {
                y--;
            }
            break;
        }
        case Direction::DOWN: {
            int allowed_offset = allow_bottom_square ? 1 : 2;

            if (y + allowed_offset < GameGrid::GAME_HEIGHT) {
                y++;
            }
            break;
        }
        case Direction::LEFT: {
            if (x - 1 >= 0) {
                x--;
            }
            break;
        }
        case Direction::RIGHT: {
            // Cursor is 2 blocks wide, so need a -1 here to account for it
            if (x + 1 < GameGrid::GAME_WIDTH - 1) {
                x++;
            }
            break;
        }
        default:
            break;
    }
    return false;
}
