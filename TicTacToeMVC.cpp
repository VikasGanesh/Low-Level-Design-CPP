#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class Stats {
private:
    int wins;
    int losses;
    int draws;
public:
    Stats(){
        wins = 0;
        losses = 0;
        draws = 0;
    }
    int getWins(){
        return wins;
    }
    
    int getLosses(){
        return losses;
    }
    
    int getDraws(){
        return draws;
    }
    
    void gameWon(){
        wins++;
    }
    
    void gameLost(){
        losses++;
    }
    
    void gameDraw(){
        draws++;
    }
};

class Player {
private:
    int playerId;
    Stats* s;
public:
    Player(int playerId){
        this->playerId = playerId;
        s = new Stats();
    }
    
    Stats* getStats(){
        return s;
    }
};

class Move {
private:
    Player* player;
    char symbol;
    int row;
    int col;
public:
    Move(Player* player, char symbol, int row, int col){
        this->player = player;
        this->symbol = symbol;
        this->row = row;
        this->col = col;
    }
    
    int getRow(){
        return row;
    }
    
    int getCol(){
        return col;
    }
    
    char getSymbol(){
        return symbol;
    }
    
    void setSymbol(char symbol){
        this->symbol = symbol;
    }
};

class TicTacToeBoard {
private:
    vector<vector<char>> board;
public:
    TicTacToeBoard(){
        board.resize(3, vector<char>(3, ' '));
    }
    
    void reset(){
        board.resize(3, vector<char>(3, ' '));
    }
    
    void makeMove(Move* move){
        board[move->getRow()][move->getCol()] = move->getSymbol();
    }
    
    char getSymbol(int row, int col){
        return board[row][col];
    }

    void setSymbol(int row, int col, char symbol){
        board[row][col] = symbol;
    }
};


enum class GameStatus{
    PROGRESS,
    COMPLETED
};

class TicTacToeModel {
private:
    TicTacToeBoard* board;
    unordered_map<Player*, char> players;
    Player* player1;
    Player* player2;
    stack<Move*> moves;
    Player* currentPlayer;
    GameStatus status;
public:
    TicTacToeModel(Player* player1, Player* player2){
        board = new TicTacToeBoard();
        players[player1] = 'X';
        players[player2] = 'Y';
        currentPlayer = player1;
        this->player1 = player1;
        this->player2 = player2;
        status = GameStatus::PROGRESS;
    }
    
void checkWinner(){
    char currentPlayerSymbol = players[currentPlayer];

    // Check rows and columns
    for (int i = 0; i < 3; i++) {
        if ((board->getSymbol(i, 0) == currentPlayerSymbol && board->getSymbol(i, 1) == currentPlayerSymbol && board->getSymbol(i, 2) == currentPlayerSymbol) ||
            (board->getSymbol(0, i) == currentPlayerSymbol && board->getSymbol(1, i) == currentPlayerSymbol && board->getSymbol(2, i) == currentPlayerSymbol)) {
            
            cout << "Player with symbol " << currentPlayerSymbol << " wins!" << endl;
            currentPlayer->getStats()->gameWon();
            Player* otherPlayer = (currentPlayer == player1) ? player2 : player1;
            otherPlayer->getStats()->gameLost();
            status = GameStatus::COMPLETED;
            return;
        }
    }

    // Check diagonals
    if ((board->getSymbol(0, 0) == currentPlayerSymbol && board->getSymbol(1, 1) == currentPlayerSymbol && board->getSymbol(2, 2) == currentPlayerSymbol) ||
        (board->getSymbol(0, 2) == currentPlayerSymbol && board->getSymbol(1, 1) == currentPlayerSymbol && board->getSymbol(2, 0) == currentPlayerSymbol)) {
        
        cout << "Player with symbol " << currentPlayerSymbol << " wins!" << endl;
        currentPlayer->getStats()->gameWon();
        Player* otherPlayer = (currentPlayer == player1) ? player2 : player1;
        otherPlayer->getStats()->gameLost();
        status = GameStatus::COMPLETED;
        return;
    }

    // Check for a draw
    if (moves.size() >= 9) {
        cout << "Game draw!\n";
        player1->getStats()->gameDraw();
        player2->getStats()->gameDraw();
        status = GameStatus::COMPLETED;
    }
}
    
    bool makeMove(int row, int col){
        if (board->getSymbol(row, col) != ' ') {
            cout << "Cell already taken!\n";
            return false;
        }
        moves.push(new Move(currentPlayer, players[currentPlayer], row, col));
        board->makeMove(moves.top());
        checkWinner();
        switchPlayer();
        return true;
    }
    
    void reset(){
        board->reset();
    }
    
    void switchPlayer(){
        currentPlayer = (currentPlayer == player1) ? player2:player1;
    }
    
    Player* getCurrentPlayer(){
        return currentPlayer;
    }
    
    void undoMove(){
        Move* move = moves.top();
        moves.pop();
        board->setSymbol(move->getRow(), move->getCol(), ' ');
    }
    
    char getSymbol(int row, int col){
        return board->getSymbol(row, col);
    }
    
    GameStatus getGameStatus(){
        return status;
    }
    
    char getPlayerSymbol() {
        return players[currentPlayer];
    }
};

class TicTacToeController;

class TicTacToeView {
private:
    TicTacToeController* controller;
public:
    TicTacToeView(){
        
    }
    void setContoller(TicTacToeController* controller){
        this->controller = controller;
    }
    void printBoard();
    void printPlayer();
};

class TicTacToeController {
private:
    TicTacToeModel* model;
    TicTacToeView* view;
public:
    TicTacToeController(TicTacToeModel* model, TicTacToeView* view){
        this->model = model;
        this->view = view;
    }
    char getCurrentSymbol(int row, int col);
    void startGame();
    char getPlayerSymbol() {
        return model->getPlayerSymbol();
    }
};


char TicTacToeController::getCurrentSymbol(int row, int col){
    return model->getSymbol(row, col);
}

void TicTacToeController::startGame(){
    while(model->getGameStatus() == GameStatus::PROGRESS){
        view->printBoard();
        view->printPlayer();
        
        int row = 0, col = 0;
        cout << "Enter your move (row and column: 1-3): ";
        cin >> row >> col;

        if (cin.fail()) {
            cin.clear(); // clear fail state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
            cout << "Invalid input! Please enter numeric values.\n";
            continue;
        }

        if (row >= 1 && row <= 3 && col >= 1 && col <= 3) {
            if (!model->makeMove(row - 1, col - 1)) {
                cout << "Try again.\n";
            }
        } else {
            cout << "Invalid move! Row and column must be between 1 and 3.\n";
        }
    }
    // Final board display at the end of the game
    view->printBoard();
}


void TicTacToeView::printBoard(){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            cout << controller->getCurrentSymbol(i, j);
            if (j < 2) cout << " | ";
        }
        cout << endl;
        if (i < 2) cout << "--+---+--" << endl;
    }
}

void TicTacToeView::printPlayer(){
    cout<<"CurrentPlayer symbol is "<<controller->getPlayerSymbol()<<endl;
}

int main() {
    Player* player1 = new Player(1);
    Player* player2 = new Player(2);
    TicTacToeModel* model = new TicTacToeModel(player1, player2);
    TicTacToeView* view = new TicTacToeView();
    TicTacToeController* controller = new TicTacToeController(model, view);
    view->setContoller(controller);
    controller->startGame();
    return 0;
}
