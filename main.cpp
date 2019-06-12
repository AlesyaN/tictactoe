#include <utility>
#include <utility>
#include <iostream>
#include <string.h>
#include <stack>
#include <fstream>
#include <ctime>
#include <sstream>

using namespace std;

#define BOARD_SIZE 11
#define MAX_NAME_LENGTH 20
#define DB "history.csv"
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

    bool stepIsCorrect(pair<int, int> step);

    void nextStep(string cmd);

    void saveStep(pair<int, int> step);

    void resetStep();

    void newGame();

    void save();

    void printHistory();

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

void Game::save() {
    ofstream fout(DB, ios::app);
    time_t now = time(nullptr);

    stack<pair<int, int>> current = steps;

    fout << now << ","
         << player1 << ","
         << player2 << ","
         << player << ",";

    while (!current.empty()) {
        fout << current.top().first << ":" << current.top().second << ",";
        current.pop();
    }
    fout << endl;
    fout.close();
}

void Game::printHistory() {
    ifstream fin(DB);
    string line;
    string word;
    cout << "HISTORY:" << endl;
    int i = 0;
    while (getline(fin, line)) {
        stringstream s(line);
        cout << i << " ";
        while (getline(s, word, ',')) {
            cout << word << " ";
        }
        cout << endl;
        i++;
    }
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

    if (player) {
        cout << player1 << " choose step: " << endl;
    } else {
        cout << player2 << " choose step: " << endl;
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

void Game::nextStep(string cmd) {
    bool correctStep = false;
    pair<char, char> step;
    pair<int, int> p;
    step.first = cmd[0];
    step.second = cmd[2];
    while (!correctStep) {
        if ((int) step.first >= (int) '0' && (int) step.first <= (int) '9') {
            p.first = step.first - '0';
        } else {
            p.first = 10 + step.first - 'A';
        }

        if ((int) step.second >= (int) '0' && (int) step.second <= (int) '9') {
            p.second = step.second - '0';
        } else {
            p.second = 10 + step.second - 'A';
        }

        correctStep = stepIsCorrect(p);
    }

    if (player) {
        board[p.first][p.second] = 'X';
    } else {
        board[p.first][p.second] = 'O';
    }
    saveStep(p);

}



void Game::play() {
    initPlayers();
    showBoard();
    bool gameOver = false;
    char winner = ' ';

    while (!gameOver) {
        string cmd;
        getline(cin, cmd);
        if (cmd == "options") {
            cout << "new game --start new game" << endl;
            cout << "reset --cancel the last step" << endl;
            cout << "save --save the current game" << endl;
            cout << "quit --quit the game" << endl;
        } else if (cmd == "new game") {
            newGame();
        } else if (cmd == "reset") {
            resetStep();
            continue;
        } else if (cmd == "save") {
            save();
            continue;
        } else if (cmd == "quit") {
            break;
        } else {
            nextStep(cmd);

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

            showBoard();
        }
    }
}

void Game::start() {
    cout << "*******   WELCOME TO TIC TAC TOE!   *******" << endl;
    cout << "Print 'options' to see all commands" << endl;

    string cmd;
    while (cmd != "exit") {
        getline(cin, cmd);

        if (cmd == "options") {
            cout << "new game --start new game" << endl;
            cout << "history --see the history of games" << endl;
            cout << "exit --quit the program" << endl;
        } else if (cmd == "history") {
            printHistory();
        } else if (cmd == "new game") {
            play();
        } else {
            cout << "No such command. Try again." << endl;
        }
    }
}


int main() {
    Game.start();
    return 0;
}