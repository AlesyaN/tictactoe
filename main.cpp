#include <utility>
#include <utility>
#include <iostream>
#include <string.h>
#include <stack>
#include <fstream>
#include <ctime>
#include <sstream>
#include <stdio.h>
#include <Windows.h>
#include <cstdlib>
#include <conio.h>

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
    int countOfFree = BOARD_SIZE * BOARD_SIZE;

    void initPlayers();

    void play();

    void showBoard();

    bool check(pair<int, int> step);

    int countCells(pair<int, int> step, int i, int j, int count);

    bool stepIsCorrect(pair<int, int> step);

    void saveStep(pair<int, int> step);

    void resetStep();

    void newGame();

    void save();

    void printHistory();

    bool initReplay(int index);

    void replay(int index);

    void setStep(pair<int, int>);

    void deleteHistory(int index);

    bool checkIfRaw();

    bool boardIsFull();

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

void Game::setStep(pair<int, int> p) {
//    set color of the board
    system("color B0");
    if (player) {
        board[p.first][p.second] = 'X';
//        decrease number of free cells when player makes step
        countOfFree--;
    } else {
        board[p.first][p.second] = 'O';
        countOfFree--;
    }
}

void Game::deleteHistory(int index) {
    ifstream is(DB);
    ofstream ofs;
    ofs.open("temp.csv", ofstream::out);
    int line_no = 0;
    string line;
    while (getline(is, line, '\n')) {
        if (line_no != index)
            ofs << line << endl;
        line_no++;
    }

    ofs.close();
    is.close();

    remove(DB);
    rename("temp.csv", DB);
}

bool Game::initReplay(int index) {
    ifstream fin(DB);
    string line;
    string word;
    int i = 0;
    while (getline(fin, line)) {
        if (i == index) {
            stack<pair<int, int>> temp;

            stringstream s(line);
            getline(s, word, ',');

            getline(s, word, ',');
            player1 = word;

            getline(s, word, ',');
            player2 = word;

            getline(s, word, ',');
            bool currentPlayer = word == "1";

            while (getline(s, word, ',')) {
                pair<int, int> step;
                stringstream str(word);

                string cell;
                getline(str, cell, ':');
                step.first = stoi(cell);

                getline(str, cell, ':');
                step.second = stoi(cell);

                temp.push(step);
            }

            player = true;
            while (!temp.empty()) {
                steps.push(temp.top());
                setStep(temp.top());
                temp.pop();
                player = !player;
            }

            countOfFree = static_cast<int>(BOARD_SIZE * BOARD_SIZE - steps.size() / 2);
            player = currentPlayer;

            fin.close();
            deleteHistory(index);

            return true;
        }
        i++;
    }
    return false;
}

void Game::replay(int index) {
    if (initReplay(index)) {
        play();
    } else {
        cout << "No game with " << index << " index";
        return;
    }
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
    cout << countOfFree << endl;
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

        getline(s, word, ',');

        time_t time = (time_t) stoi(word);
        tm *ltm = localtime(&time);
        cout << ltm->tm_hour << ":"
             << ltm->tm_min << ":"
             << ltm->tm_sec << " "
             << ltm->tm_mday << "."
             << 1 + ltm->tm_mon << "."
             << 1900 + ltm->tm_year << " ";
        while (getline(s, word, ',')) {
            cout << word << " ";
        }
        cout << endl;
        i++;
    }

    cout << "\n Print 'replay *number*' to replay the game" << endl;
    cout << "\n Print 'delete *number*' to delete the game" << endl;
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

bool Game::check(pair<int, int> step) {
    int count = 0;
    //diagonals and vertical are checked
    for (int j = -1; j < 2; j++) {
        count = countCells(step, -1, j, count);
        if (count >= 4) {
            return true;
        } else {
            count = countCells(step, 1, -j, count);
            if (count >= 4) {
                return true;
            }
        }
        count = 0;
    }
    //horizontal check
    count = countCells(step, 0, 1, count);
    if (count >= 4) {
        return true;
    } else {
        count = countCells(step, 0, -1, count);
        return (count >= 4);
    }
}

//    count number of same chars in line
int Game::countCells(pair<int, int> step, int i, int j, int count) {
    static pair<int, int> currentStep;
    if (board[step.first][step.second] == board[step.first + i][step.second + j]) {
        count++;
        currentStep.first = step.first + i;
        currentStep.second = step.second + j;
        countCells(currentStep, i, j, count);
    } else {
        return count;
    }
}

bool Game::stepIsCorrect(pair<int, int> step) {
    if (step.first < BOARD_SIZE && step.second < BOARD_SIZE && (int) board[step.first][step.second] == 0) {
        return true;
    } else {
//        change color if player made mistake
        system("color C0");
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
//        increase number of free cells if step is reset
        countOfFree++;
    } else {
        cout << "No steps to reset" << endl;
    }
}


void Game::play() {
    showBoard();

    while (true) {
        string cmd;
        cin.sync();
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

            pair<char, char> step;
            pair<int, int> p;
            step.first = cmd[0];
            step.second = cmd[2];

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

            if (stepIsCorrect(p)) {
                saveStep(p);
                setStep(p);
                player = !player;
            }
            if (check(p)) {
//                change color of the console if game is over
                system("color E0");
                cout << "Game over! ";
                if (!player) {
                    cout << player1;
                } else {
                    cout << player2;
                }
                cout << " won!\nPrint 'new game' to start a new game" << endl;
                return;
            } else if (checkIfRaw()) {
                cout << "It is raw!" << endl;
            }
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
            initPlayers();
            play();
        } else {
            stringstream s(cmd);
            string subCmd;
            string num;
            getline(s, subCmd, ' ');
            if (subCmd == "replay") {
                getline(s, num, ' ');
                replay(stoi(num));
            } else if (subCmd == "delete") {
                getline(s, num, ' ');
                deleteHistory(stoi(num));
                printHistory();
            } else {
                cout << "No such command. Try again." << endl;
            }
        }
    }
}

//  check if there are empty cells
bool Game::boardIsFull() {
    return countOfFree == 0;
}

//  check if it is raw
bool Game::checkIfRaw() {
    return boardIsFull();
}


int main() {
//    set main color of the game
    system("color B0");
    Game.start();
    return 0;
}