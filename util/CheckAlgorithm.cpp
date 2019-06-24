#include "CheckAlgorithm.h"

bool CheckAlgorithm::check(Board board, pair<int,int> checkingStep) {
    int count = 0;
    pair<int, int> step = checkingStep;

    //diagonals and vertical
    for (int j = -1; j < 2; j++) {
        count = countCells(board, step, -1, j, count);
        if (count >= 4) {
            return true;
        } else {
            count = countCells(board, step, 1, -j, count);
            if (count >= 4) {
                return true;
            }
        }
        count = 0;
    }
    //horizontal
    count = countCells(board, step, 0, 1, count);
    if (count >= 4) {
        return true;
    } else {
        count = countCells(board, step, 0, -1, count);
        return (count >= 4);
    }
}

int CheckAlgorithm::countCells(Board board, pair<int, int> step, int i, int j, int count) {
    static pair<int, int> currentStep;
    if (board.boardArray[step.first][step.second] == board.boardArray[step.first + i][step.second + j]) {
        count++;
        currentStep.first = step.first + i;
        currentStep.second = step.second + j;
        return countCells(board, currentStep, i, j, count);
    } else {
        return count;
    }
}

CheckAlgorithm::CheckAlgorithm() {
}

bool CheckAlgorithm::checkIfRaw(Board board) {
    return board.steps.size() == BOARD_SIZE * BOARD_SIZE;
}



