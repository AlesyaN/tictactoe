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
                int moveVal = minimax(0, false, alpha, beta);
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

int AIPlayer::minimax(int depth, bool isMax, int alpha, int beta) {
    auto board = gameBoard.boardArray;
    int score = estimate();
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

    if (isMax) {
        int bestVal = -999;
        for (int i = start(gameBoard.steps.top().first); i <= end(gameBoard.steps.top().first); i++) {
            for (int j = start(gameBoard.steps.top().second); j <= end(gameBoard.steps.top().second); j++) {
                if (board[i][j] == (char) 0) {
                    board[i][j] = 'O';
                    int val = minimax(depth + 1, !isMax, alpha, beta);
                    bestVal = max(bestVal, val);
                    alpha = max(bestVal, alpha);
                    board[i][j] = (char) 0;
                    if (beta <= alpha)
                        break;
                }
                if (beta <= alpha)
                    break;
            }
        }
        return bestVal;
    } else {
        int bestVal = 999;
        for (int i = start(gameBoard.steps.top().first); i <= end(gameBoard.steps.top().first); i++) {
            for (int j = start(gameBoard.steps.top().second); j <= end(gameBoard.steps.top().second); j++) {
                if (board[i][j] == (char) 0) {
                    board[i][j] = 'X';
                    int val = minimax(depth + 1, !isMax, alpha, beta);
                    bestVal = min(bestVal, val);
                    beta = min(bestVal, beta);
                    board[i][j] = (char) 0;
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

int AIPlayer::estimate() {
    CheckAlgorithm ca;
    auto board = gameBoard.boardArray;
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            pair<int, int> p(i, j);
            if (board[i][j] == 'O' && ca.check(gameBoard, p)) {
                return 10;
            }
        }
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            pair<int, int> p(i, j);
            if (board[i][j] == 'X' && ca.check(gameBoard, p)) {
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










