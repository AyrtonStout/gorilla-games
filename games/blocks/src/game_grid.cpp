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

    active_block left_active_block = { .block = left_block, .x = x, .y = y };
    active_block right_active_block = { .block = right_block, .x = x + 1, .y = y };

    add_active_block(left_active_block);
    add_active_block(right_active_block);

    check_for_matches();
}

vector<active_block> GameGrid::check_direction(int x, int y, Direction direction) {
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
        return vector<active_block>();
    }

    auto found_blocks = vector<active_block>();
    found_blocks.push_back(active_block { .block = current_block, .x = x, .y = y });

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
            found_blocks.push_back(active_block { .block = compare_block, .x = x, .y = y });
        } else {
            break;
        }
    }

    if (found_blocks.size() > 2) {
        return found_blocks;
    } else {
        return vector<active_block>();
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
                    popping_block.block->transition_to_state(BlockAction::FLASHING_1);
                    bool block_added = add_active_block(popping_block);

                    if (block_added) {
                        combo_size++;

                        if (popping_block.block->chainable) {
                            chained = true;
                        }

                        if (popping_block.y < top_pop_coordinate
                        || (popping_block.y == top_pop_coordinate && popping_block.x < left_pop_coordinate)) {
                            left_pop_coordinate = popping_block.x;
                            top_pop_coordinate = popping_block.y;
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

bool GameGrid::add_active_block(active_block active_block) {
    int block_id = active_block.block->get_id();
    if (active_blocks_ids.count(block_id) == 0) {
        active_blocks_ids.insert(block_id);
        active_blocks.push_back(active_block);

        return true;
    }
    return false;
}

void GameGrid::add_new_falling_blocks(vector<active_block> &new_actions, int x, int y, bool chainable) {
    // Go up from the moved / popped block, telling every contiguous block above it to get ready to fall
    for (int y_to_check = y - 1; y_to_check >= 0; y_to_check--) {
        auto block_to_check = blocks[y_to_check][x];
        if (block_to_check->deleted || block_to_check->block_action != BlockAction::NONE) {
            break;
        } else {
            block_to_check->transition_to_state(BlockAction::POP_FLOAT);
            block_to_check->chainable = chainable;
            new_actions.push_back({ .block = block_to_check, .x = x, .y = y_to_check });
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
                swap(blocks[y - 1][x], blocks[y][x]);
            }
        }

        auto new_blocks = board_generator.generate_row();
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
        // TODO make new blocks appear
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
    auto new_actions = vector<active_block>();

    // Tell all our blocks to update themselves before we do anything extra.
    // Otherwise the order that blocks complete stuff like falling gets really weird
    for (auto active_block : active_blocks) {
        active_block.block->update();
    }

    for (int i = 0; i < active_blocks.size(); i++) {

        int x = active_blocks[i].x;
        int y = active_blocks[i].y;
        auto current_block = active_blocks[i].block;

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
                swap(blocks[y][x], blocks[y][x + 1]);
                match_check_needed = true;
            }

            // FIXME: Although it won't ever happen right now, this logic expects the SLIDE_RIGHT block to finish before SLIDE_LEFT does
            // Otherwise, the x coordinate of the SLIDE_LEFT block won't have been updated in the swap
            // It probably makes sense to have "update groups" for multiple blocks that resolve all at once
            int new_x = action == BlockAction::SLIDE_RIGHT ? x + 1 : x - 1;

            // Check x + 1 here because we have since swapped positions to the right
            if (!current_block->deleted &&  y + 1 < GAME_HEIGHT && !blocks[y + 1][new_x]->can_prevent_falling()) {
                // The block slid over a cliff. Add a watcher to make sure it will fall later
                current_block->transition_to_state(BlockAction::SLIDE_FLOAT);
                new_actions.push_back({ .block = current_block, .x = new_x, .y = y });
            }

            add_new_falling_blocks(new_actions, x, y, false);
        } else if (action == BlockAction::FLOATING) {
            if (y + 1 < GAME_HEIGHT && !blocks[y + 1][x]->can_prevent_falling()) {
                current_block->transition_to_state(BlockAction::FALLING);
                new_actions.push_back({ .block = current_block, .x = x, .y = y });
            }

        } else if (action == BlockAction::FALLING) {
            if (blocks[y + 1][x]->deleted) {
                swap(blocks[y][x], blocks[y + 1][x]);
                match_check_needed = true;

                // Keep falling if there continues to be no blocks below us
                if (y + 2 < GAME_HEIGHT && !blocks[y + 2][x]->can_prevent_falling()) {
                    current_block->transition_to_state(BlockAction::FALLING);
                    new_actions.push_back({ .block = current_block, .x = x, .y = y + 1 });
                }
            }
        } else if (action == BlockAction::FLASHING_1) {
            // no op for now
        } else if (action == BlockAction::FLASHING_2) {
            add_new_falling_blocks(new_actions, x, y, true);
        }

        current_block->complete_action();
        active_blocks.erase(active_blocks.begin() + i); // Delete both this and the next block...
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
