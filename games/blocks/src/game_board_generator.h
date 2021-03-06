#ifndef HELLO_GAME_BOARD_GENERATOR_H
#define HELLO_GAME_BOARD_GENERATOR_H

#include <vector>
#include <stdint.h>
#include "block.h"

using namespace std;

class GameBoardGenerator {
public:
    vector<vector<shared_ptr<Block>>> generate_pattern();
    vector<shared_ptr<Block>> generate_bottom_row();

private:
    vector<shared_ptr<Block>> last_generated_row;

    vector<vector<vector<uint8_t>>> generation_patterns = {
            {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 1, 0, 0, 0, 0, 0 },
                    { 1, 0, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 1, 0, 1, 1, 1, 0 },
                    { 1, 0, 1, 1, 1, 0 },
                    { 1, 0, 1, 1, 1, 0 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 1, 0, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 1, 1, 1, 0, 1 },
                    { 0, 1, 1, 1, 0, 1 },
                    { 0, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 1, 0, 0, 0 },
                    { 0, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 1, 0, 1, 1, 0, 1 },
                    { 1, 0, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 1, 0, 0, 0, 0 },
                    { 0, 1, 0, 1, 1, 0 },
                    { 1, 1, 0, 1, 1, 0 },
                    { 1, 1, 0, 1, 1, 0 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 1, 0, 0, 1 },
                    { 1, 1, 1, 1, 0, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
                    { 1, 1, 1, 1, 1, 1 },
            }, {
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 0, 0, 0, 0, 0, 0 },
                    { 1, 0, 1, 1, 1, 1 },
                    { 1, 0, 1, 1, 1, 1 },
                    { 1, 0, 1, 1, 1, 1 },
                    { 1, 0, 1, 1, 1, 1 },
                    { 1, 0, 1, 1, 1, 1 },
                    { 1, 0, 1, 1, 1, 1 },
            }
    };
};


#endif //HELLO_GAME_BOARD_GENERATOR_H
