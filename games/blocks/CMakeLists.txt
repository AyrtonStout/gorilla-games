project(hello)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_executable(${PROJECT_NAME} src/main.cpp src/game_cursor.cpp src/game_cursor.h src/direction.h src/block.cpp src/block.h src/game_grid.cpp src/game_grid.h src/game_state.cpp src/game_state.h src/sdl2_runner.cpp src/sdl2_runner.h src/frame_timing.h src/game_board_generator.cpp src/game_board_generator.h)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
