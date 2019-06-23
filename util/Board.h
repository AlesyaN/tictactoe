#ifndef FINAL_TICTACTOE_BOARD_H
#define FINAL_TICTACTOE_BOARD_H

#include <utility>
#include <iostream>
#include <string.h>
#include <stack>
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

#define BOARD_SIZE 11

//entity of game board, keeps 2D array and stack of all steps, including last user step
class Board {

public:
    char boardArray[BOARD_SIZE][BOARD_SIZE];
    stack<pair<int, int>> steps;
};
#endif
