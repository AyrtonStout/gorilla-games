#include <climits>
#include <iostream>
#include "direction.h"
#include "game_grid.h"
#include "game_board_generator.h"

GameGrid::GameGrid() {
    board_generator = GameBoardGenerator();
    game_cursor = GameCursor();

    blocks = board_generator.generate_pattern();
}

void GameGrid::swap_panels(int x, int y) {
    auto left_block = blocks[y][x];
    auto right_block = blocks[y][x + 1];

    if (left_block->block_action != BlockAction::NONE || right_block->block_action != BlockAction::NONE) {
        return;
    }

    if (left_block->deleted && right_block->deleted) {
        return;
    }

    left_block->transition_to_state(BlockAction::SLIDE_RIGHT);
    right_block->transition_to_state(BlockAction::SLIDE_LEFT);

    add_active_block(blocks[y][x]);
    add_active_block(blocks[y][x + 1]);

    check_for_matches();
}

void GameGrid::swap_blocks(int x1, int y1, int x2, int y2) {
    blocks[y1][x1]->x = x2;
    blocks[y1][x1]->y = y2;

    blocks[y2][x2]->x = x1;
    blocks[y2][x2]->y = y1;

    swap(blocks[y1][x1], blocks[y2][x2]);
}

vector<shared_ptr<Block>> GameGrid::check_direction(int x, int y, Direction direction) {
    int dx = 0, dy = 0;
    switch (direction) {
        case Direction::UP: dy = -1; break;
        case Direction::LEFT: dx = -1; break;
        case Direction::RIGHT: dx = 1; break;
        case Direction::DOWN: dy = -1; break;
        default: throw std::invalid_argument("Unsupported direction");
    }

    auto current_block = blocks[y][x];
    if (!current_block->can_be_matched_with()) {
        return vector<shared_ptr<Block>>();
    }

    auto found_blocks = vector<shared_ptr<Block>>();
    found_blocks.push_back(current_block);

    while (true) {
        x += dx;
        y += dy;

        if (x < 0 || y < 0 || y > blocks.size() - 1 || x > blocks[0].size() - 1) {
            break;
        }

        auto compare_block = blocks[y][x];
        if (!compare_block->can_be_matched_with()) {
            break;
        }

        if (compare_block->block_type == current_block->block_type) {
            found_blocks.push_back(compare_block);
        } else {
            break;
        }
    }

    if (found_blocks.size() > 2) {
        return found_blocks;
    } else {
        return vector<shared_ptr<Block>>();
    }
}

void GameGrid::check_for_matches() {
    int size_to_check = are_bottom_blocks_accessible() ? blocks.size() : blocks.size() - 1;

    int left_pop_coordinate = INT_MAX;
    int top_pop_coordinate = INT_MAX;
    int combo_size = 0;

    bool chained = false;

    for (int y = 0; y < size_to_check; y++) {
        for (int x = 0; x < blocks[0].size(); x++) {
            Direction directions[] = { Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT };

            for (auto direction : directions) {
                auto found_blocks = check_direction(x, y, direction);

                for (const auto& popping_block : found_blocks) {
                    popping_block->transition_to_state(BlockAction::FLASHING_1);
                    bool block_added = add_active_block(popping_block);

                    if (block_added) {
                        combo_size++;

                        if (popping_block->chainable) {
                            chained = true;
                        }

                        if (popping_block->y < top_pop_coordinate
                        || (popping_block->y == top_pop_coordinate && popping_block->x < left_pop_coordinate)) {
                            left_pop_coordinate = popping_block->x;
                            top_pop_coordinate = popping_block->y;
                        }
                    }
                }
            }
        }
    }

    if (combo_size > 3 && left_pop_coordinate > -1) {
        special_pop_indications.push_back(
                {
                        .size = combo_size,
                        .pop_type = PopType::COMBO,
                        .x = left_pop_coordinate,
                        .y = top_pop_coordinate,
                        .frames_remaining = 60,
                        .both_special_triggered = chained
                }
        );
    }

    if (chained) {
        special_pop_indications.push_back(
                {
                        .size = ++current_chain,
                        .pop_type = PopType::CHAIN,
                        .x = left_pop_coordinate,
                        .y = top_pop_coordinate,
                        .frames_remaining = 60,
                        .both_special_triggered = combo_size > 3
                }
        );
    }
}

bool GameGrid::add_active_block(shared_ptr<Block> active_block) {
    int block_id = active_block->get_id();
    if (active_blocks_ids.count(block_id) == 0) {
        active_blocks_ids.insert(block_id);
        active_blocks.push_back(active_block);

        return true;
    }
    return false;
}

void GameGrid::add_new_falling_blocks(vector<shared_ptr<Block>> &new_actions, int x, int y, bool chainable) {
    // Go up from the moved / popped block, telling every contiguous block above it to get ready to fall
    for (int y_to_check = y - 1; y_to_check >= 0; y_to_check--) {
        auto block_to_check = blocks[y_to_check][x];
        if (block_to_check->deleted || block_to_check->block_action != BlockAction::NONE) {
            break;
        } else {
            block_to_check->transition_to_state(BlockAction::POP_FLOAT);
            block_to_check->chainable = chainable;
            new_actions.push_back(block_to_check);
        }
    }
}

void GameGrid::update() {
    handle_block_updates();
    handle_stack_increase();
    handle_combo_indications();
}

void GameGrid::handle_stack_increase() {
    // The stack does not rise if anything at all is happening
    if (active_blocks.size() > 0) {
        return;
    }

    // stack_raise_requested / stack_raise_active are when the user forces a stack raise
    // After the user requests the stack be raised, it cannot be stopped until the stack raise is over
    // During this time, the stack raises one pixel per frame
    if (stack_raise_requested) {
        stack_raise_active = true;
    }

    stack_increase_frame++;

    if (stack_raise_active || stack_increase_frame >= frames_per_stack_increase) {
        stack_increase_height++;
        stack_increase_frame = 0;
    }

    if (stack_increase_height == Block::BLOCK_SIZE) {
        // This is the point that the bottom blocks have "moved up" all the way and are now accessible
        for (int x = 0; x < blocks[0].size(); x++) {
            blocks[blocks.size() - 1][x]->inaccessible = false;
        }
        check_for_matches();
        stack_raise_active = false;
    } else if (stack_increase_height > Block::BLOCK_SIZE) {
        // This is the frame right after, where we spawn in new blocks
        stack_increase_height = 1;

        // Move all the blocks up
        for (int y = 1; y < blocks.size(); y++) {
            for (int x = 0; x < blocks[0].size(); x++) {
                swap_blocks(x, y, x, y - 1);
            }
        }

        auto new_blocks = board_generator.generate_bottom_row();
        // Replace the blocks in the bottom row with new blocks
        for (int x = 0; x < new_blocks.size(); x++) {
            new_blocks[x]->inaccessible = true;
            blocks[blocks.size() - 1][x] = new_blocks[x];
        }

        // Keep moving the cursor up with the blocks (unless it is at the top. In which case we let it get bumped down)
        if (game_cursor.y > 0) {
            game_cursor.y--;
        }

        // TODO game over condition
    }

}

int GameGrid::get_stack_increase_height() {
    return stack_increase_height;
}

void GameGrid::handle_combo_indications() {
    for (int i = 0; i < special_pop_indications.size(); i++) {
        special_pop_indications[i].frames_remaining--;
        if (special_pop_indications[i].frames_remaining == 0) {
            special_pop_indications.erase(special_pop_indications.begin() + i);
            i--;
        }
    }
}

void GameGrid::handle_block_updates() {
    if (active_blocks.size() == 0) {
        return;
    }

    bool match_check_needed = false;
    auto new_actions = vector<shared_ptr<Block>>();

    // Tell all our blocks to update themselves before we do anything extra.
    // Otherwise the order that blocks complete stuff like falling gets really weird
    for (auto active_block : active_blocks) {
        active_block->update();
    }

    for (int i = 0; i < active_blocks.size(); i++) {

        auto current_block = active_blocks[i];
        int x = current_block->x;
        int y = current_block->y;

        if (!current_block->is_action_done()) {
            continue; // This block is still doing an action. Do nothing more with it
        }

        auto action = current_block->block_action;

        // We now know that the block is done doing its action. Do some stuff additional based off what kind of action it finished
        if (action == BlockAction::SLIDE_RIGHT || action == BlockAction::SLIDE_LEFT) {
            // The block finished sliding to its new location. Swap it with the block next to it
            // Don't check for SLIDE_LEFT, as we only need to do the swap for one completed block
            // Empty blocks still count as blocks, so there will always be a SLIDE_LEFT and SLIDE_RIGHT
            if (action == BlockAction::SLIDE_RIGHT) {
                swap_blocks(x, y, x + 1, y);
                match_check_needed = true;
            }

            if (
                    !current_block->deleted
                    && current_block->y + 1 < GAME_HEIGHT
                    && !blocks[current_block->y + 1][current_block->x]->can_prevent_falling()
               ) {
                // The block slid over a cliff. Add a watcher to make sure it will fall later
                current_block->transition_to_state(BlockAction::SLIDE_FLOAT);
                new_actions.push_back(current_block);
            }

            add_new_falling_blocks(new_actions, current_block->x, current_block->y, false);
        } else if (action == BlockAction::FLOATING) {
            if (y + 1 < GAME_HEIGHT && !blocks[y + 1][x]->can_prevent_falling()) {
                current_block->transition_to_state(BlockAction::FALLING);
                new_actions.push_back(current_block);
            }

        } else if (action == BlockAction::FALLING) {
            if (blocks[y + 1][x]->deleted) {
                swap_blocks(x, y, x, y + 1);
                match_check_needed = true;

                // Keep falling if there continues to be no blocks below us
                if (current_block->y + 1 < GAME_HEIGHT && !blocks[current_block->y + 1][x]->can_prevent_falling()) {
                    current_block->transition_to_state(BlockAction::FALLING);
                    new_actions.push_back(current_block);
                }
            }
        } else if (action == BlockAction::FLASHING_1) {
            // no op for now
        } else if (action == BlockAction::FLASHING_2) {
            add_new_falling_blocks(new_actions, x, y, true);
        }

        current_block->complete_action();
        active_blocks.erase(active_blocks.begin() + i);
        active_blocks_ids.erase(current_block->get_id());
        i--;
    }

    if (match_check_needed) {
        check_for_matches();
    }

    // TODO this probably has unintended behavior if a block is floating when it gets matched with other blocks...
    // Add any new actions so that we keep watch over them
    for (const auto& new_action : new_actions) {
        add_active_block(new_action);
    }
}

void GameGrid::move_cursor(Direction direction) {
    game_cursor.move(direction, are_bottom_blocks_accessible());
}

bool GameGrid::are_bottom_blocks_accessible() {
    return stack_increase_height == Block::BLOCK_SIZE;
}

vector<special_pop_indication> GameGrid::get_combo_indications() {
    return special_pop_indications;
}
