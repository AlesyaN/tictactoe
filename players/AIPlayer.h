//
// Created by Asus on 22.06.2019.
//
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
    AIPlayer();

    Board gameBoard;

    AIPlayer(Board &gameBoard);

    void initPlayer();

    void playWithAI();

    pair<int, int> findBestMove();

    int start(int i);

    int end(int i);

    int minimax(int depth, bool isMax, int alpha, int beta);

    int evaluate();

    bool isMovesLeft();


};

#endif
