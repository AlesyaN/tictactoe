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
    int* getStep(bool player);
    bool stepIsCorrect(int* step);
    void setStep(int* step, bool player);
public:
    void start();

} Game;


//get names of players
void Game::initPlayers() {
    string name1;
    string name2;
    cout << "Print first player's name: "<< endl;
    getline(cin, name1);

    cout << "Print second player's name: "<< endl;
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


bool Game::stepIsCorrect(int *step) {
    if (step[0] < BOARD_SIZE && step[1] < BOARD_SIZE && (int) board[step[0]][step[1]] == 0) {
        return true;
    } else {
        cout << "Wrong step!" << endl;
        return false;
    }
}


int* Game::getStep(bool player) {
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

        if ((int)step[0] >= (int)'0' && (int)step[0] <= (int)'9') {
            intStep[0] = step[0] - '0';
        } else {
            intStep[0] = 10 + step[0] - 'A';
        }

        if ((int)step[2] >= (int)'0' && (int)step[2] <= (int)'9') {
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

void Game::play() {
    showBoard();
    bool gameOver = false;
    bool player = true;

    while (!gameOver) {
        int* steps = getStep(player);
        setStep(steps, player);
        showBoard();
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