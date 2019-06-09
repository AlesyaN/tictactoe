#include <utility>

#include <utility>

#include <iostream>
#include <string.h>

using namespace std;

#define BOARD_SIZE 11
#define MAX_NAME_LENGTH 20

//main class of the game
class Game {
private:
    char board[BOARD_SIZE][BOARD_SIZE]{};
    string player1;
    string player2;

    void initPlayers();

    void play();

    void showBoard();

    char checkLines();

    char checkColumns();

    char checkDiagonals();

    bool win(char winner);

    int *getStep(bool player);

    bool stepIsCorrect(int *step);

    void setStep(int *step, bool player);

public:
    void start();

} Game;


//get names of players
void Game::initPlayers() {
    string name1;
    string name2;
    cout << "Print first player's name: " << endl;
    getline(cin, name1);

    cout << "Print second player's name: " << endl;
    getline(cin, name2);
    player1 = std::move(name1);
    player2 = std::move(name2);
}


void Game::showBoard() {

    //first line with numbers
    cout << "  ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (9 < i) {
            cout << static_cast<char>('A' + i - 10) << "|";
        } else {
            cout << i << "|";
        }
    }
    cout << endl;

    //the board
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (9 < i) {
            cout << static_cast<char>('A' + i - 10) << "|";
        } else {
            cout << i << "|";
        }

        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << board[i][j] << '|';
        }

        cout << endl;
    }
}

char Game::checkLines() {

    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) { /* check lines */
            if (board[i][j] == board[i][j + 1] &&
                board[i][j] == board[i][j + 2] &&
                board[i][j] == board[i][j + 3] &&
                board[i][j] == board[i][j + 4]) {
                return board[i][j];
            }
        }
    }
    return ' ';
}

char Game::checkColumns() {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) { /* check columns */
            if (board[i][j] == board[i + 1][j] &&
                board[i][j] == board[i + 2][j] &&
                board[i][j] == board[i + 3][j] &&
                board[i][j] == board[i + 4][j]) {
                return board[i][j];
            }
        }
    }
    return ' ';
}

char Game::checkDiagonals() {
    for (int i = 0; i < 11; i++) {
        for (int j = 0; j < 11; j++) { /* check diagonals */
            if (board[i][j] == board[i + 1][j + 1] &&
                board[i + 1][j + 1] == board[i + 2][j + 2] &&
                board[i + 2][j + 2] == board[i + 3][j + 3] &&
                board[i + 3][j + 3] == board[i + 4][j + 4]) {
                return board[i][j];
            }
        }
    }
    return ' ';

//    for (int i = 0; i < 11; i++) {
//        for (int j = 0; j < 11; j++) {
//            if (board[i][j + 4] == board[i + 1][j + 3] &&
//                board[i + 1][j + 3] == board[i + 2][j + 2] &&
//                board[i + 2][j + 2] == board[i + 3][j + 1] &&
//                board[i + 3][j + 1] == board[i + 4][j]) {
//                cout << board[0][2] << endl;
//                return board[0][2];
//            }
//        }
//    }
}

bool Game::stepIsCorrect(int *step) {
    if (step[0] < BOARD_SIZE && step[1] < BOARD_SIZE && (int) board[step[0]][step[1]] == 0) {
        return true;
    } else {
        cout << "Wrong step!" << endl;
        return false;
    }
}

int *Game::getStep(bool player) {
    bool correctStep = false;
    char step[3];
    static int intStep[2];
    while (!correctStep) {
        if (player) {
            cout << player1 << " choose step: " << endl;
        } else {
            cout << player2 << " choose step: " << endl;
        }
        cin.get(step, 4);
        cin.ignore();

        if ((int) step[0] >= (int) '0' && (int) step[0] <= (int) '9') {
            intStep[0] = step[0] - '0';
        } else {
            intStep[0] = 10 + step[0] - 'A';
        }

        if ((int) step[2] >= (int) '0' && (int) step[2] <= (int) '9') {
            intStep[1] = step[2] - '0';
        } else {
            intStep[1] = 10 + step[2] - 'A';
        }

        correctStep = stepIsCorrect(intStep);
    }
    return intStep;
}

void Game::setStep(int *step, bool player) {
    if (player) {
        board[step[0]][step[1]] = 'X';
    } else {
        board[step[0]][step[1]] = 'O';
    }
}

bool Game::win(char winner) {
    if (winner == 'X') {
        cout << player1 << " WON!";
        return true;
    } else if (winner == 'O') {
        cout << player2 << " WON!";
        return true;
    }
    return false;
}

void Game::play() {
    showBoard();
    bool gameOver = false;
    bool player = true;
    char winner = ' ';

    while (!gameOver) {
        int *steps = getStep(player);
        setStep(steps, player);
        showBoard();

        winner = checkLines();
        if (win(winner)) {
            gameOver = true;
        }

        winner = checkColumns();
        if (win(winner)) {
            gameOver = true;
        }

        winner = checkDiagonals();
        if (win(winner)) {
            gameOver = true;
        }

        player = !player;
    }
}


void Game::start() {
    cout << "*******   WELCOME TO TIC TAC TOE!   *******" << endl;
    initPlayers();
    play();
}


int main() {
    Game.start();
}