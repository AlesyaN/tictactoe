#include "AIPlayer.h"
#include "../util/CheckAlgorithm.h"

pair<int, int> AIPlayer::getAIMove() {
    auto board = gameBoard.boardArray;
    int bestVal = -1000;
    pair<int, int> bestMove;
    bestMove.first = -1;
    bestMove.second = -1;
    int alpha = -1000;
    int beta = 1000;

    for (int i = start(gameBoard.steps.top().first); i <= end(gameBoard.steps.top().first); i++) {
        for (int j = start(gameBoard.steps.top().second); j <= end(gameBoard.steps.top().second); j++) {
            if (board[i][j] == (char) 0) {
                board[i][j] = 'O';
                int moveVal = minimax(gameBoard, 0, false, alpha, beta);
                board[i][j] = (char) 0;
                if (moveVal > bestVal) {
                    bestMove.first = i;
                    bestMove.second = j;
                    bestVal = moveVal;
                }
            }
        }
    }
    return bestMove;
}


int AIPlayer::start(int i) {
    if (i == 0) return 0;
    if (i == BOARD_SIZE - 1) return i - 2;
    return i - 1;
}

int AIPlayer::end(int i) {
    if (i == 0) return 2;
    if (i == BOARD_SIZE - 1) return i;
    return i + 1;
}

int AIPlayer::minimax(Board b, int depth, bool isMax, int alpha, int beta) {
    int score = estimate(b);
    if (depth == 9)
        return 0;

    if (score == 10) {
        return score - depth;
    }

    if (score == -10) {
        return score + depth;
    }

    if (!isFreeCellsLeft()) {
        return 0;
    }

    if (isMax) { //max move
        int bestVal = -999;
        for (int i = start(b.steps.top().first); i <= end(b.steps.top().first); i++) {
            for (int j = start(b.steps.top().second); j <= end(b.steps.top().second); j++) {
                if (b.boardArray[i][j] == (char) 0) {
                    b.boardArray[i][j] = 'O';
                    int val = minimax(b, depth + 1, !isMax, alpha, beta);
                    bestVal = max(bestVal, val);
                    alpha = max(bestVal, alpha);
                    b.boardArray[i][j] = (char) 0;
                    if (beta <= alpha)
                        break;
                }
                if (beta <= alpha)
                    break;
            }
        }
        return bestVal;
    } else {//min move
        int bestVal = 999;
        for (int i = start(b.steps.top().first); i <= end(b.steps.top().first); i++) {
            for (int j = start(b.steps.top().second); j <= end(b.steps.top().second); j++) {
                if (b.boardArray[i][j] == (char) 0) {
                    b.boardArray[i][j] = 'X';
                    int val = minimax(b, depth + 1, !isMax, alpha, beta);
                    bestVal = min(bestVal, val);
                    beta = min(bestVal, beta);
                    b.boardArray[i][j] = (char) 0;
                    if (beta <= alpha)
                        break;
                }
                if (beta <= alpha)
                    break;
            }
        }
        return bestVal;
    }
}

int AIPlayer::estimate(Board b) {
    CheckAlgorithm ca;
    auto board = b.boardArray;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            pair<int, int> p(i, j);
            if (board[i][j] == 'O' && ca.check(b, p)) {
                return 10;
            }
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            pair<int, int> p(i, j);
            if (board[i][j] == 'X' && ca.check(b, p)) {
                return -10;
            }
        }
    }
    return 0;
}

bool AIPlayer::isFreeCellsLeft() {
    return gameBoard.steps.size() < BOARD_SIZE * BOARD_SIZE;
}

AIPlayer::AIPlayer(Board &gameBoard) : gameBoard(gameBoard) {}

AIPlayer::AIPlayer() {}










