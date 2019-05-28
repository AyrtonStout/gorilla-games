#include <iostream>
#include <unordered_set>
#include "direction.h"
#include "game_grid.h"

GameGrid::GameGrid() {
    for (int y = 0; y < GameGrid::GAME_HEIGHT; y++) {
        blocks.emplace_back();
        for (int x = 0; x < GameGrid::GAME_WIDTH; x++) {
            shared_ptr<Block> ptr(new Block());
            blocks[y].push_back(ptr);
        }
    }
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

    active_blocks.push_back(left_active_block);
    active_blocks.push_back(right_active_block);

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

        if (compare_block->get_block_type() == current_block->get_block_type()) {
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
    auto popping_block_ids = unordered_set<int>();
    auto popping_blocks = vector<active_block>();

    for (int y = 0; y < blocks.size(); y++) {
        for (int x = 0; x < blocks[0].size(); x++) {
            Direction directions[] = { Direction::UP, Direction::LEFT, Direction::DOWN, Direction::RIGHT };

            for (auto direction : directions) {
                auto found_blocks = check_direction(x, y, direction);

                for (const auto& popping_block : found_blocks) {
                    int block_id = popping_block.block->get_id();
                    if (popping_block_ids.count(block_id) == 0) {
                        popping_block_ids.insert(block_id);
                        popping_blocks.push_back(popping_block);
                    }
                }
            }
        }
    }

    for (const auto& popping_block : popping_blocks) {
        popping_block.block->transition_to_state(BlockAction::FLASHING_1);
        active_blocks.push_back(popping_block);
    }
}

void GameGrid::update() {
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
            // Go up from the popped block, telling every contiguous block above it to get ready to fall
            for (int y_to_check = y - 1; y_to_check >= 0; y_to_check--) {
                auto block_to_check = blocks[y_to_check][x];
                if (block_to_check->deleted || block_to_check->block_action != BlockAction::NONE) {
                    break;
                } else {
                    block_to_check->transition_to_state(BlockAction::POP_FLOAT);
                    new_actions.push_back({ .block = block_to_check, .x = x, .y = y_to_check });
                }
            }
        }

        current_block->complete_action();
        active_blocks.erase(active_blocks.begin() + i); // Delete both this and the next block...
        i--;
    }

    if (match_check_needed) {
        check_for_matches();
    }

    // TODO this probably has unintended behavior if a block is floating when it gets matched with other blocks...
    // Add any new actions so that we keep watch over them
    for (const auto& new_action : new_actions) {
        active_blocks.push_back(new_action);
    }
}
