cmake_minimum_required(VERSION 3.13)
project(tic_tac_toe)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

add_executable(tic_tac_toe Game.cpp players/AIPlayer.cpp players/AIPlayer.h util/Board.h util/CheckAlgorithm.cpp util/CheckAlgorithm.h)