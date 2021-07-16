cmake_minimum_required(VERSION 3.17)
project(UltimateTicTacToe C)

set(CMAKE_C_STANDARD 99)

include_directories( includes )
find_package(SDL2 REQUIRED SDL2)

add_executable(UltimateTicTacToe main.c rendering.c logic.c)

target_link_libraries(UltimateTicTacToe PRIVATE SDL2::SDL2)

