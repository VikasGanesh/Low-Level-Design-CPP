#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include <map>

// Forward declarations
class ChessView;
class ChessController;

// ===== OBSERVER PATTERN =====
class Observer {
public:
    virtual ~Observer() = default;
    virtual void update(const std::string& event, const std::string& data) = 0;
};

class Observable {
private:
    std::vector<Observer*> observers;
public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    
    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
    
    void notifyObservers(const std::string& event, const std::string& data = "") {
        for (auto* observer : observers) {
            observer->update(event, data);
        }
    }
};

// ===== MODEL LAYER =====
enum class PieceType { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY };
enum class Color { WHITE, BLACK, NONE };

// Position class for board coordinates
struct Position {
    int row, col;
    
    Position(int r = 0, int c = 0) : row(r), col(c) {}
    
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    std::string toString() const {
        return std::string(1, 'a' + col) + std::to_string(8 - row);
    }
};

// Chess piece class
class ChessPiece {
private:
    PieceType type;
    Color color;
    bool hasMoved;

public:
    ChessPiece(PieceType t = PieceType::EMPTY, Color c = Color::NONE) 
        : type(t), color(c), hasMoved(false) {}
    
    PieceType getType() const { return type; }
    Color getColor() const { return color; }
    bool hasMovedBefore() const { return hasMoved; }
    void setMoved() { hasMoved = true; }
    
    bool isEmpty() const { return type == PieceType::EMPTY; }
    
    char getSymbol() const {
        if (isEmpty()) return '.';
        
        char symbols[] = {'P', 'R', 'N', 'B', 'Q', 'K'};
        char symbol = symbols[static_cast<int>(type)];
        return (color == Color::WHITE) ? symbol : tolower(symbol);
    }
    
    std::string getName() const {
        std::string names[] = {"Pawn", "Rook", "Knight", "Bishop", "Queen", "King", "Empty"};
        return names[static_cast<int>(type)];
    }
};

// Move class to represent a chess move
class Move {
public:
    Position from, to;
    ChessPiece capturedPiece;
    bool isValid;
    
    Move(Position f, Position t) : from(f), to(t), isValid(false) {}
    
    std::string getNotation() const {
        return from.toString() + "-" + to.toString();
    }
};

// Chess Board class (part of Model)
class ChessBoard {
private:
    ChessPiece board[8][8];
    
    void initializePieces() {
        // Initialize pawns
        for (int col = 0; col < 8; col++) {
            board[1][col] = ChessPiece(PieceType::PAWN, Color::BLACK);
            board[6][col] = ChessPiece(PieceType::PAWN, Color::WHITE);
        }
        
        // Initialize other pieces
        PieceType backRow[] = {PieceType::ROOK, PieceType::KNIGHT, PieceType::BISHOP, 
                              PieceType::QUEEN, PieceType::KING, PieceType::BISHOP, 
                              PieceType::KNIGHT, PieceType::ROOK};
        
        for (int col = 0; col < 8; col++) {
            board[0][col] = ChessPiece(backRow[col], Color::BLACK);
            board[7][col] = ChessPiece(backRow[col], Color::WHITE);
        }
        
        // Initialize empty squares
        for (int row = 2; row < 6; row++) {
            for (int col = 0; col < 8; col++) {
                board[row][col] = ChessPiece();
            }
        }
    }

public:
    ChessBoard() {
        initializePieces();
    }
    
    ChessPiece& getPiece(const Position& pos) {
        return board[pos.row][pos.col];
    }
    
    const ChessPiece& getPiece(const Position& pos) const {
        return board[pos.row][pos.col];
    }
    
    void setPiece(const Position& pos, const ChessPiece& piece) {
        board[pos.row][pos.col] = piece;
    }
    
    bool isOccupied(const Position& pos) const {
        return !getPiece(pos).isEmpty();
    }
    
    bool isOccupiedByColor(const Position& pos, Color color) const {
        return isOccupied(pos) && getPiece(pos).getColor() == color;
    }
    
    // Basic move validation for demonstration
    bool isValidMove(const Move& move, Color playerColor) const {
        if (!move.from.isValid() || !move.to.isValid()) return false;
        if (move.from == move.to) return false;
        
        const ChessPiece& piece = getPiece(move.from);
        if (piece.isEmpty() || piece.getColor() != playerColor) return false;
        
        // Can't capture own piece
        if (isOccupiedByColor(move.to, playerColor)) return false;
        
        // Basic piece-specific validation (simplified)
        return validatePieceMove(piece, move);
    }
    
private:
    bool validatePieceMove(const ChessPiece& piece, const Move& move) const {
        int rowDiff = move.to.row - move.from.row;
        int colDiff = move.to.col - move.from.col;
        
        switch (piece.getType()) {
            case PieceType::PAWN: {
                int direction = (piece.getColor() == Color::WHITE) ? -1 : 1;
                if (colDiff == 0) { // Forward move
                    if (rowDiff == direction && !isOccupied(move.to)) return true;
                    if (rowDiff == 2 * direction && !piece.hasMovedBefore() && 
                        !isOccupied(move.to) && !isOccupied(Position(move.from.row + direction, move.from.col))) 
                        return true;
                } else if (abs(colDiff) == 1 && rowDiff == direction) { // Capture
                    return isOccupied(move.to) && getPiece(move.to).getColor() != piece.getColor();
                }
                return false;
            }
            case PieceType::ROOK:
                return (rowDiff == 0 || colDiff == 0) && isPathClear(move);
            case PieceType::BISHOP:
                return (abs(rowDiff) == abs(colDiff)) && isPathClear(move);
            case PieceType::QUEEN:
                return (rowDiff == 0 || colDiff == 0 || abs(rowDiff) == abs(colDiff)) && isPathClear(move);
            case PieceType::KING:
                return abs(rowDiff) <= 1 && abs(colDiff) <= 1;
            case PieceType::KNIGHT:
                return (abs(rowDiff) == 2 && abs(colDiff) == 1) || (abs(rowDiff) == 1 && abs(colDiff) == 2);
            default:
                return false;
        }
    }
    
    bool isPathClear(const Move& move) const {
        int rowDir = (move.to.row > move.from.row) ? 1 : (move.to.row < move.from.row) ? -1 : 0;
        int colDir = (move.to.col > move.from.col) ? 1 : (move.to.col < move.from.col) ? -1 : 0;
        
        Position current = Position(move.from.row + rowDir, move.from.col + colDir);
        while (current.row != move.to.row || current.col != move.to.col) {
            if (isOccupied(current)) return false;
            current.row += rowDir;
            current.col += colDir;
        }
        return true;
    }
};

// Chess Game Model (Main Model Class)
class ChessModel : public Observable {
private:
    ChessBoard board;
    Color currentPlayer;
    std::vector<Move> moveHistory;
    bool gameOver;
    std::string gameStatus;

public:
    ChessModel() : currentPlayer(Color::WHITE), gameOver(false), gameStatus("Game in progress") {}
    
    // Core game operations
    bool makeMove(const Position& from, const Position& to) {
        Move move(from, to);
        
        if (!board.isValidMove(move, currentPlayer)) {
            notifyObservers("INVALID_MOVE", "Invalid move: " + move.getNotation());
            return false;
        }
        
        // Execute the move
        move.capturedPiece = board.getPiece(to);
        ChessPiece movingPiece = board.getPiece(from);
        
        board.setPiece(to, movingPiece);
        board.setPiece(from, ChessPiece()); // Empty square
        board.getPiece(to).setMoved();
        
        moveHistory.push_back(move);
        
        // Switch players
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
        
        // Notify observers
        std::string moveData = move.getNotation() + " by " + 
                              (movingPiece.getColor() == Color::WHITE ? "White" : "Black");
        notifyObservers("MOVE_MADE", moveData);
        notifyObservers("BOARD_UPDATED");
        
        return true;
    }
    
    bool undoMove() {
        if (moveHistory.empty()) {
            notifyObservers("UNDO_FAILED", "No moves to undo");
            return false;
        }
        
        Move lastMove = moveHistory.back();
        moveHistory.pop_back();
        
        // Restore the move
        ChessPiece movingPiece = board.getPiece(lastMove.to);
        board.setPiece(lastMove.from, movingPiece);
        board.setPiece(lastMove.to, lastMove.capturedPiece);
        
        // Switch players back
        currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
        
        notifyObservers("MOVE_UNDONE", "Last move undone");
        notifyObservers("BOARD_UPDATED");
        return true;
    }
    
    void resetGame() {
        board = ChessBoard();
        currentPlayer = Color::WHITE;
        moveHistory.clear();
        gameOver = false;
        gameStatus = "Game in progress";
        
        notifyObservers("GAME_RESET", "New game started");
        notifyObservers("BOARD_UPDATED");
    }
    
    // Getters for View
    const ChessBoard& getBoard() const { return board; }
    Color getCurrentPlayer() const { return currentPlayer; }
    const std::vector<Move>& getMoveHistory() const { return moveHistory; }
    bool isGameOver() const { return gameOver; }
    const std::string& getGameStatus() const { return gameStatus; }
    
    std::string getCurrentPlayerName() const {
        return (currentPlayer == Color::WHITE) ? "White" : "Black";
    }
};

// ===== VIEW LAYER =====
// Abstract View Interface
class ChessView : public Observer {
protected:
    ChessController* controller;

public:
    virtual ~ChessView() = default;
    virtual void setController(ChessController* ctrl) { controller = ctrl; }
    virtual void displayBoard(const ChessBoard& board) = 0;
    virtual void displayGameStatus(const std::string& status) = 0;
    virtual void displayError(const std::string& error) = 0;
    virtual void displayMessage(const std::string& message) = 0;
    virtual void startGameLoop() = 0;
};

// Console View Implementation
class ConsoleChessView : public ChessView {
public:
    void displayBoard(const ChessBoard& board) override {
        std::cout << "\n   a b c d e f g h\n";
        std::cout << "  ┌─────────────────┐\n";
        
        for (int row = 0; row < 8; row++) {
            std::cout << (8 - row) << " │ ";
            for (int col = 0; col < 8; col++) {
                std::cout << board.getPiece(Position(row, col)).getSymbol() << " ";
            }
            std::cout << "│ " << (8 - row) << "\n";
        }
        
        std::cout << "  └─────────────────┘\n";
        std::cout << "   a b c d e f g h\n\n";
    }
    
    void displayGameStatus(const std::string& status) override {
        std::cout << "Status: " << status << std::endl;
    }
    
    void displayError(const std::string& error) override {
        std::cout << "ERROR: " << error << std::endl;
    }
    
    void displayMessage(const std::string& message) override {
        std::cout << "INFO: " << message << std::endl;
    }
    
    void startGameLoop() override;
    
    // Observer pattern implementation
    void update(const std::string& event, const std::string& data) override {
        if (event == "BOARD_UPDATED") {
            // Board will be displayed in main game loop
        } else if (event == "MOVE_MADE") {
            displayMessage("Move made: " + data);
        } else if (event == "INVALID_MOVE") {
            displayError(data);
        } else if (event == "GAME_RESET") {
            displayMessage(data);
        } else if (event == "MOVE_UNDONE") {
            displayMessage(data);
        } else if (event == "UNDO_FAILED") {
            displayError(data);
        }
    }

private:
    Position parsePosition(const std::string& pos) {
        if (pos.length() != 2) return Position(-1, -1);
        
        int col = pos[0] - 'a';
        int row = 8 - (pos[1] - '0');
        
        return Position(row, col);
    }
    
    void showMenu() {
        std::cout << "\n=== Chess Game Commands ===\n";
        std::cout << "1. Make move (e.g., 'e2 e4')\n";
        std::cout << "2. 'undo' - Undo last move\n";
        std::cout << "3. 'reset' - Start new game\n";
        std::cout << "4. 'quit' - Exit game\n";
        std::cout << "============================\n\n";
    }
};

// ===== CONTROLLER LAYER =====
class ChessController {
private:
    std::unique_ptr<ChessModel> model;
    std::unique_ptr<ChessView> view;

public:
    ChessController() {
        model = std::make_unique<ChessModel>();
        view = std::make_unique<ConsoleChessView>();
        
        // Set up the relationships
        view->setController(this);
        model->addObserver(view.get());
    }
    
    // Controller methods called by View
    void makeMove(const std::string& fromStr, const std::string& toStr) {
        Position from = parsePosition(fromStr);
        Position to = parsePosition(toStr);
        
        if (!from.isValid() || !to.isValid()) {
            view->displayError("Invalid position format. Use format like 'e2' or 'a1'");
            return;
        }
        
        model->makeMove(from, to);
    }
    
    void undoMove() {
        model->undoMove();
    }
    
    void resetGame() {
        model->resetGame();
    }
    
    void displayCurrentGame() {
        view->displayBoard(model->getBoard());
        view->displayGameStatus("Current player: " + model->getCurrentPlayerName());
    }
    
    // Start the application
    void run() {
        view->startGameLoop();
    }
    
private:
    Position parsePosition(const std::string& pos) {
        if (pos.length() != 2) return Position(-1, -1);
        
        int col = pos[0] - 'a';
        int row = 8 - (pos[1] - '0');
        
        return Position(row, col);
    }
};

// Implementation of ConsoleChessView::startGameLoop()
void ConsoleChessView::startGameLoop() {
    std::cout << "Welcome to Chess Game (MVC Pattern Demo)!\n";
    showMenu();
    
    std::string input;
    while (true) {
        controller->displayCurrentGame();
        
        std::cout << "Enter command: ";
        std::getline(std::cin, input);
        
        if (input == "quit") {
            std::cout << "Thanks for playing!\n";
            break;
        } else if (input == "undo") {
            controller->undoMove();
        } else if (input == "reset") {
            controller->resetGame();
        } else if (input == "help") {
            showMenu();
        } else {
            // Parse move command (e.g., "e2 e4")
            size_t spacePos = input.find(' ');
            if (spacePos != std::string::npos) {
                std::string from = input.substr(0, spacePos);
                std::string to = input.substr(spacePos + 1);
                controller->makeMove(from, to);
            } else {
                displayError("Invalid command. Type 'help' for commands.");
            }
        }
    }
}

// ===== MAIN APPLICATION =====
int main() {
    try {
        ChessController controller;
        controller.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
