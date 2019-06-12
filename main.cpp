#include <utility>
#include <utility>
#include <iostream>
#include <string.h>
#include <stack>

using namespace std;

#define BOARD_SIZE 11
#define MAX_NAME_LENGTH 20

//main class of the game
class Game {
private:
    char board[BOARD_SIZE][BOARD_SIZE]{};
    string player1;
    string player2;
    stack<pair<int, int>> steps;
    bool player = true;

    void initPlayers();

    void play();

    void showBoard();

    char checkLines();

    char checkColumns();

    char checkDiagonals();

    bool win(char winner);

    pair<int, int> getStep();

    bool stepIsCorrect(pair<int, int> step);

    void setStep(pair<int, int> step, bool player);

    void saveStep(pair<int, int> step);

    void resetStep();

    void newGame();

public:

    void start();


} Game;


//get names of players
void Game::initPlayers() {
    string name1;
    string name2;
    cout << "Print first player's name: " << endl;
    cin.sync();
    getline(cin, name1);

    cout << "Print second player's name: " << endl;
    getline(cin, name2);
    player1 = std::move(name1);
    player2 = std::move(name2);
}

void Game::newGame() {
    *this = Game();
    start();
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

bool Game::stepIsCorrect(pair<int, int> step) {
    if (step.first < BOARD_SIZE && step.second < BOARD_SIZE && (int) board[step.first][step.second] == 0) {
        return true;
    } else {
        cout << "Wrong step!" << endl;
        return false;
    }
}

pair<int, int> Game::getStep() {
    bool correctStep = false;
    string cmd;
    char step[2];
    pair<int, int> p;
    while (!correctStep) {
        if (player) {
            cout << player1 << " choose step: " << endl;
        } else {
            cout << player2 << " choose step: " << endl;
        }
        cin >> cmd;
        if (cmd == "reset") {
            resetStep();
            continue;
        }
        if (cmd == "newgame") {
            newGame();
            continue;
        }
        step[0] = cmd[0];
        cin >> step[1];

        if ((int) step[0] >= (int) '0' && (int) step[0] <= (int) '9') {
            p.first = step[0] - '0';
        } else {
            p.first = 10 + step[0] - 'A';
        }

        if ((int) step[1] >= (int) '0' && (int) step[1] <= (int) '9') {
            p.second = step[1] - '0';
        } else {
            p.second = 10 + step[1] - 'A';
        }

        correctStep = stepIsCorrect(p);
    }
    return p;
}

void Game::saveStep(pair<int, int> step) {
    steps.push(step);
}

void Game::resetStep() {
    if (!steps.empty()) {
        pair<int, int> p = steps.top();
        board[p.first][p.second] = 0;
        player = !player;
        showBoard();
        steps.pop();
    } else {
        cout << "No steps to reset" << endl;
    }
}

void Game::setStep(pair<int, int> step, bool player) {
    if (player) {
        board[step.first][step.second] = 'X';
    } else {
        board[step.first][step.second] = 'O';
    }
    saveStep(step);
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
    char winner = ' ';

    while (!gameOver) {
        pair<int, int> steps = getStep();
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