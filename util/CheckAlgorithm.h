#ifndef FINAL_TICTACTOE_CHECKALGORITHM_H
#define FINAL_TICTACTOE_CHECKALGORITHM_H

using namespace std;

#define BOARD_SIZE 11

#include "algorithm"
#include "../util/Board.h"

class CheckAlgorithm {
private:

    int countCells(Board board, pair<int, int> step, int i, int j, int count);

public:
    CheckAlgorithm();

    bool checkIfRaw(Board board);

    bool check(Board board);
};


#endif
