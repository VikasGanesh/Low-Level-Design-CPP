#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

// Prototype Interface
class Monster {
public:
    virtual ~Monster() = default;
    virtual unique_ptr<Monster> clone() const = 0;
    virtual void showType() const = 0;
};

// Concrete Class: Well-Known Monster
class WellKnownMonster : public Monster {
    string type;
public:
    WellKnownMonster(string t) : type(move(t)) {}

    // Copy Constructor
    WellKnownMonster(const WellKnownMonster& other) : type(other.type) {}

    // Clone method using unique_ptr
    unique_ptr<Monster> clone() const override {
        return make_unique<WellKnownMonster>(*this);  // Correct way to clone
    }

    void showType() const override {
        cout << "WellKnownMonster: " << type << endl;
    }
};

// Concrete Class: Player-Generated Monster
class DynamicPlayerGeneratedMonster : public Monster {
    string type;
public:
    DynamicPlayerGeneratedMonster(string t) : type(move(t)) {}

    // Copy Constructor
    DynamicPlayerGeneratedMonster(const DynamicPlayerGeneratedMonster& other) : type(other.type) {}

    // Clone method using unique_ptr
    unique_ptr<Monster> clone() const override {
        return make_unique<DynamicPlayerGeneratedMonster>(*this);  // Correct way to clone
    }

    void showType() const override {
        cout << "DynamicPlayerGeneratedMonster: " << type << endl;
    }
};

// Monster Registry to manage prototypes
class MonsterRegistry {
    unordered_map<string, unique_ptr<Monster>> registry;
public:
    void registerMonster(string key, unique_ptr<Monster> prototype) {
        registry[key] = move(prototype);
    }

    unique_ptr<Monster> getMonster(const string& key) {
        if (registry.find(key) != registry.end()) {
            return registry[key]->clone();  // Properly returning a new object
        }
        return nullptr;
    }
};

int main() {
    MonsterRegistry registry;

    // Register monster prototypes
    registry.registerMonster("dragon", make_unique<WellKnownMonster>("Fire Dragon"));
    registry.registerMonster("goblin", make_unique<WellKnownMonster>("Forest Goblin"));
    registry.registerMonster("player_monster", make_unique<DynamicPlayerGeneratedMonster>("Custom Beast"));

    // Create new monsters using the registry
    auto monster1 = registry.getMonster("dragon");
    auto monster2 = registry.getMonster("goblin");
    auto monster3 = registry.getMonster("player_monster");

    // Display the created monsters
    if (monster1) monster1->showType();
    if (monster2) monster2->showType();
    if (monster3) monster3->showType();

    return 0;
}
