#include <utility>
#include <iostream>
#include <string.h>
#include <stack>
#include <fstream>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

#include "../util/Board.h"
#ifndef FINAL_TICTACTOE_AIPLAYER_H
#define FINAL_TICTACTOE_AIPLAYER_H

class AIPlayer {

public:

    //instance of game field
    Board gameBoard;

    pair<int, int> getAIMove();

    //constructors
    AIPlayer();
    AIPlayer(Board &gameBoard);

private:
    int start(int i);

    int end(int i);

    int minimax(int depth, bool isMax, int alpha, int beta);

    int estimate();

    bool isFreeCellsLeft();
};

#endif
