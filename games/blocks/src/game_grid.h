#ifndef HELLO_GAME_GRID_H
#define HELLO_GAME_GRID_H


#include <memory>
#include <unordered_set>
#include <vector>
#include "block.h"
#include "game_cursor.h"
#include "game_board_generator.h"

using namespace std;

struct active_block {
    shared_ptr<Block> block;
    int x;
    int y;
};

enum class PopType {
    COMBO, CHAIN
};

struct special_pop_indication {
    int size;
    PopType pop_type;
    int x;
    int y;
    int frames_remaining;
    bool both_special_triggered; // A combo and a chain off the same event
};

class GameGrid {
public:
    GameGrid();
    GameCursor game_cursor;
    vector<vector<shared_ptr<Block>>> blocks;
    bool stack_raise_requested = false;

    static const int GAME_HEIGHT = 12;
    static const int GAME_WIDTH = 6;

    static const int MAX_COMBO = 19; // Does this make sense to be in this header file?
    static const int MAX_CHAIN = 19; // Ditto

    int get_stack_increase_height();
    vector<special_pop_indication> get_combo_indications();
    vector<special_pop_indication> get_chain_indications();
    void move_cursor(Direction direction);
    void swap_panels(int x, int y);
    void update();

private:
    vector<active_block> active_blocks;
    vector<special_pop_indication> special_pop_indications;
    unordered_set<int> active_blocks_ids;
    GameBoardGenerator board_generator;

    int stack_increase_height = Block::BLOCK_SIZE; // How much the stack has increased in pixels. It resets to 0 when it hits Block::BLOCK_SIZE
    int stack_increase_frame = 0; // Our current frame counter for stack increase. Stack goes up when this hits "frames_per_stack_increase"
    int frames_per_stack_increase = 20; // How many frames it takes for the stack to go up by one

    int current_chain = 1;

    bool stack_raise_active = false;

    void check_for_matches();
    bool add_active_block(active_block active_block);
    vector<active_block> check_direction(int x, int y, Direction direction);

    void add_new_falling_blocks(vector<active_block> &new_actions, int y, int x, bool chainable);
    void handle_block_updates();
    void handle_stack_increase();
    void handle_combo_indications();

    bool are_bottom_blocks_accessible();
};


#endif
