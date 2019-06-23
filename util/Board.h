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

class Board {
public:
    char boardArray[BOARD_SIZE][BOARD_SIZE];
    pair<int, int> lastMove;
    stack<pair<int, int>> steps;
};

#endif
