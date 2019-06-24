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

#include <algorithm>
#include "players/AIPlayer.h"
#include "util/CheckAlgorithm.h"

using namespace std;

#define DB "history.csv"

class Game {
private:
    Board b;
    string player1;
    string player2;
    AIPlayer aiPlayer;
    CheckAlgorithm checkAlgorithm;


    bool player = true;

    void initPlayers();

    void play();

    void showBoard();
    void whoIsNext();

    bool stepIsCorrect(pair<int, int> step);

    void resetStep();

    void newGame();

    void save();

    void printHistory();

    bool initReplay(int index);

    void replay(int index);

    void setStep(pair<int, int>);

    void deleteHistory(int index);

    void initPlayer();

    void playWithAI();

    pair<int, int> getPlayerMove(string input);

public:

    void start();

} Game;


//get names of players
void Game::initPlayers() {
    string name1;
    string name2;
    do {
        cout << "Print first player's name: " << endl;
        cin.sync();
        getline(cin, name1);
    } while (name1.empty());

    do {
        cout << "Print second player's name: " << endl;
        getline(cin, name2);
    } while (name1.empty());
    b = Board();
    player1 = std::move(name1);
    player2 = std::move(name2);
}

void Game::initPlayer() {
    cout << "Print your name: " << endl;
    cin.sync();
    getline(cin, player1);
    b = Board();
    aiPlayer = AIPlayer(b);
    player2 = "computer";
}

void Game::newGame() {
    *this = Game();
    start();
}

void Game::setStep(pair<int, int> p) {
    if (player) {
        b.boardArray[p.first][p.second] = 'X';
    } else {
        b.boardArray[p.first][p.second] = 'O';
    }
    b.steps.push(p);
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
                b.steps.push(temp.top());
                setStep(temp.top());
                temp.pop();
                player = !player;
            }

//            player = b.steps.size() % 2 == 0;

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
        if (player2 != "computer") {
            play();
        } else {
            playWithAI();
        }
    } else {
        cout << "No game with " << index << " index";
        return;
    }
}

void Game::save() {
    ofstream fout(DB, ios::app);
    time_t now = time(nullptr);

    stack<pair<int, int>> current = b.steps;

    fout << now << ","
         << player1 << ","
         << player2 << ",";

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

    cout << "  ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (9 < i) {
            cout << static_cast<char>('A' + i - 10) << "|";
        } else {
            cout << i << "|";
        }
    }
    cout << endl;

    for (int i = 0; i < BOARD_SIZE; i++) {
        if (9 < i) {
            cout << static_cast<char>('A' + i - 10) << "|";
        } else {
            cout << i << "|";
        }

        for (int j = 0; j < BOARD_SIZE; j++) {
            cout << b.boardArray[i][j] << '|';
        }

        cout << endl;
    }
}
void Game::whoIsNext(){
    if (player) {
        system("color E0");
        cout << player1 << " choose step: " << endl;
    } else {
        system("color B0");
        cout << player2 << " choose step: " << endl;
    }
}

bool Game::stepIsCorrect(pair<int, int> step) {
    if (step.first < BOARD_SIZE && step.second < BOARD_SIZE && (int) b.boardArray[step.first][step.second] == 0) {
        return true;
    } else {
        system("color C0");
        cout << "Wrong step!" << endl;
        return false;
    }
}

void Game::resetStep() {
    if (!b.steps.empty()) {
        pair<int, int> p = b.steps.top();
        b.boardArray[p.first][p.second] = 0;
        player = !player;
        showBoard();
        whoIsNext();
        b.steps.pop();
    } else {
        cout << "No steps to reset" << endl;
    }
}


void Game::play() {
    showBoard();
    whoIsNext();

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
            pair<int, int> p = getPlayerMove(cmd);
            if (checkAlgorithm.check(b, b.steps.top())) {
                system("color A0");
                showBoard();
                cout << "Game over! ";
                if (!player) {
                    cout << player1;
                } else {
                    cout << player2;
                }
                cout << " won!\nPrint 'new game' to start a new game" << endl;
                return;
            } else if (checkAlgorithm.checkIfRaw(b)) {
                cout << "It is raw!" << endl;
            }
            showBoard();
            whoIsNext();
        }
    }
}

void Game::playWithAI() {
    showBoard();
    whoIsNext();

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
            resetStep();
            continue;
        } else if (cmd == "save") {
            save();
            continue;
        } else if (cmd == "quit") {
            break;
        } else {
            pair<int, int> p = getPlayerMove(cmd);
            if (p.first != -1 && p.second != -1) {
                if (checkAlgorithm.check(b, b.steps.top())) {
                    system("color A0");
                    showBoard();
                    cout << "Game over! ";
                    if (!player) {
                        cout << player1;
                    } else {
                        cout << "computer";
                    }
                    cout << " won!\nPrint 'new game' to start a new game" << endl;
                    return;
                } else if (checkAlgorithm.checkIfRaw(b)) {
                    cout << "It is raw!" << endl;
                }
                showBoard();
                whoIsNext();
                aiPlayer.gameBoard = b;
                pair<int, int> aiStep = aiPlayer.getAIMove();
                b.boardArray[aiStep.first][aiStep.second] = 'O';
                setStep(aiStep);
                player = !player;
                showBoard();
                whoIsNext();
            }


        }
    }
}

pair<int, int> Game::getPlayerMove(string input) {
    pair<char, char> step;
    pair<int, int> p;
    step.first = input[0];
    step.second = input[2];

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
        setStep(p);
        player = !player;
        return p;
    } else {
        p.first = -1;
        p.second = -1;
        return p;
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
            string mode;
            bool gameStarted = false;
            while (!gameStarted) {
                cout << "Choose game mode: \n 0 - play with computer \n 1 - play with friend" << endl;
                cin >> mode;
                if (mode == "0") {
                    initPlayer();
                    playWithAI();
                    gameStarted = true;
                } else if (mode == "1") {
                    initPlayers();
                    play();
                    gameStarted = true;
                } else cout << "Incorrect input" << endl;
            }

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

int main() {
    Game.start();
    return 0;
}