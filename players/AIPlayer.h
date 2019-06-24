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
    int start(int i); //finds first position around last user Move

    int end(int i); //last position

    int minimax(int depth, bool isMax, int alpha, int beta);

    int estimate(); //estimates the chances of winning

    bool isFreeCellsLeft(); //checks game board for emptiness
};

#endif
