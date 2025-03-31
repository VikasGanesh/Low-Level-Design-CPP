#include <iostream>
#include <vector>
using namespace std;

// Memento: Stores the game state (level & health)
class Memento {
    int level;
    int health;
public:
    Memento(int lvl, int hp) : level(lvl), health(hp) {}
    int getLevel() { return level; }
    int getHealth() { return health; }
};

// Originator: Player whose state we save/restore
class Player {
    int level;
    int health;
public:
    Player(int lvl, int hp) : level(lvl), health(hp) {}

    void play(int lvl, int hp) {  // Simulates playing and progressing
        level = lvl;
        health = hp;
        cout << "Player reached level " << level << " with health " << health << endl;
    }

    Memento save() {  // Saves current state
        cout << "Saving progress: Level " << level << ", Health " << health << endl;
        return Memento(level, health);
    }

    void restore(Memento memento) {  // Restores state from Memento
        level = memento.getLevel();
        health = memento.getHealth();
        cout << "Restored to Level " << level << " with Health " << health << endl;
    }
};

// Caretaker: Manages saved game states
class GameSaveManager {
    vector<Memento> saves;
public:
    void saveGame(Memento memento) {
        saves.push_back(memento);
        cout << "Game saved!\n";
    }

    Memento loadLastSave() {
        if (!saves.empty()) {
            Memento lastSave = saves.back();
            saves.pop_back();
            return lastSave;
        }
        cout << "No saved game found!\n";
        return Memento(1, 100);  // Default state if no save exists
    }
};

// Main function demonstrating the Memento Pattern
int main() {
    Player player(1, 100);
    GameSaveManager saveManager;

    // Playing and saving progress
    player.play(5, 80);
    saveManager.saveGame(player.save());

    player.play(8, 40);
    saveManager.saveGame(player.save());

    // Game over, restoring last save
    cout << "\nGame Over! Restoring last save...\n";
    player.restore(saveManager.loadLastSave());

    // Game over again, restoring to an earlier save
    cout << "\nGame Over Again! Restoring to an earlier save...\n";
    player.restore(saveManager.loadLastSave());

    return 0;
}
