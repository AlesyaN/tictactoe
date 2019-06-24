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


//get names of players in 2 players mode
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
//get names of players in play with computer mode
void Game::initPlayer() {
    cout << "Print your name: " << endl;
    cin.sync();
    getline(cin, player1);
    b = Board();
    aiPlayer = AIPlayer(b);
    player2 = "computer";
}
//creating a new Game object to start a new game
void Game::newGame() {
    *this = Game();
    start();
}

//save step to the board and to the stack with steps
void Game::setStep(pair<int, int> p) {
    if (player) {
        b.boardArray[p.first][p.second] = 'X';
    } else {
        b.boardArray[p.first][p.second] = 'O';
    }
    b.steps.push(p);
}


void Game::deleteHistory(int index) {
    //open output and input streams
    ifstream is(DB);
    ofstream ofs;

    //create temporary file
    ofs.open("temp.csv", ofstream::out);

    //copy from the original file everything except a line with certain index
    int line_no = 0;
    string line;
    while (getline(is, line, '\n')) {
        if (line_no != index)
            ofs << line << endl;
        line_no++;
    }

    //close streams
    ofs.close();
    is.close();

    //delete old file
    remove(DB);
    //rename new file
    rename("temp.csv", DB);
}

bool Game::initReplay(int index) {
    //open input stream
    ifstream fin(DB);
    string line;
    string word;
    int i = 0;
    //read every line in a file
    while (getline(fin, line)) {
        //init replay for a line with certain index
        if (i == index) {
            //temporary stack
            stack<pair<int, int>> temp;

            stringstream s(line);
            getline(s, word, ','); //skip first word in a line - timestamp

            getline(s, word, ',');
            player1 = word; //initializing name of 1 player

            getline(s, word, ',');
            player2 = word; //initializing name of 2 player

            //fill temporary stack backwards
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

            //fill an original stack
            while (!temp.empty()) {
                b.steps.push(temp.top()); //fill board
                setStep(temp.top());
                temp.pop();
                player = !player;
            }

            fin.close(); //close input stream
            deleteHistory(index); //delete line from history

            return true; //true if this game initialized
        }
        i++;
    }
    return false; //there is no game with this index
}

void Game::replay(int index) {
    if (initReplay(index)) { //if there is a game with such index, initialize
        if (player2 != "computer") {
            play(); //start game with partner
        } else {
            playWithAI(); //start game with ai
        }
    } else {
        cout << "No game with " << index << " index";
        return;
    }
}

void Game::save() {
    ofstream fout(DB, ios::app); //open output stream
    time_t now = time(nullptr); //get current time

    stack<pair<int, int>> current = b.steps; //get steps from a board

    //print time and names of players
    fout << now << ","
         << player1 << ","
         << player2 << ",";

    //print history of steps from stack
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
    //read every line in a file
    while (getline(fin, line)) {
        stringstream s(line);
        cout << i << " "; //index of game

        getline(s, word, ',');

        //convert timestamp to a normal date and time
        time_t time = (time_t) stoi(word);
        tm *ltm = localtime(&time);
        cout << ltm->tm_hour << ":"
             << ltm->tm_min << ":"
             << ltm->tm_sec << " "
             << ltm->tm_mday << "."
             << 1 + ltm->tm_mon << "."
             << 1900 + ltm->tm_year << " ";

        //print history of steps
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
    //first line with numbers and letters
    cout << "  ";
    for (int i = 0; i < BOARD_SIZE; i++) {
        if (9 < i) {
            cout << static_cast<char>('A' + i - 10) << "|";
        } else {
            cout << i << "|";
        }
    }
    cout << endl;

    //other lines
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
    //if the step is not out of bounds and not already reserved
    if (step.first < BOARD_SIZE && step.second < BOARD_SIZE && (int) b.boardArray[step.first][step.second] == 0) {
        return true;
    } else {
        system("color C0");
        cout << "Wrong step!" << endl;
        return false;
    }
}


void Game::resetStep() {
    if (!b.steps.empty()) { //if there is steps in stack
        pair<int, int> p = b.steps.top();
        b.boardArray[p.first][p.second] = 0; //delete this step from board
        player = !player; //change current player
        showBoard();
        whoIsNext();
        b.steps.pop(); //delete this step from stack
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
    system("color F0");
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