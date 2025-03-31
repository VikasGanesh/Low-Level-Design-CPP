#include <iostream>
#include <vector>
using namespace std;

// Forward declarations
class MenuItem;
class Ingredient;

// Visitor Interface
class Visitor {
public:
    virtual void visit(MenuItem* item) = 0;
    virtual void visit(Ingredient* ingredient) = 0;
};

// Element Interface
class Visitable {
public:
    virtual void accept(Visitor* v) = 0;
};

// Concrete Element: MenuItem
class MenuItem : public Visitable {
private:
    string name;
    int calories, protein, carbs;

public:
    MenuItem(string name, int cal, int prot, int carb)
        : name(name), calories(cal), protein(prot), carbs(carb) {}

    string getName() const { return name; }
    int getCalories() const { return calories; }
    int getProtein() const { return protein; }
    int getCarbs() const { return carbs; }

    void accept(Visitor* v) override { v->visit(this); }
};

// Concrete Element: Ingredient
class Ingredient : public Visitable {
private:
    string name;
    int calories;

public:
    Ingredient(string name, int cal) : name(name), calories(cal) {}

    string getName() const { return name; }
    int getCalories() const { return calories; }

    void accept(Visitor* v) override { v->visit(this); }
};

// Concrete Visitor: Nutrition Calculator
class NutritionVisitor : public Visitor {
public:
    void visit(MenuItem* item) override {
        cout << "Menu Item: " << item->getName() << "\n";
        cout << " - Calories: " << item->getCalories() << "\n";
        cout << " - Protein: " << item->getProtein() << "g\n";
        cout << " - Carbs: " << item->getCarbs() << "g\n";
        cout << "------------------------\n";
    }

    void visit(Ingredient* ingredient) override {
        cout << "Ingredient: " << ingredient->getName() << "\n";
        cout << " - Calories: " << ingredient->getCalories() << "\n";
        cout << "------------------------\n";
    }
};

// Client Code
int main() {
    vector<Visitable*> items = {
        new MenuItem("Pancakes", 500, 8, 80),
        new MenuItem("Salad", 200, 5, 20),
        new Ingredient("Olive Oil", 120),
        new Ingredient("Cheese", 90)
    };

    NutritionVisitor visitor;

    cout << "Nutrition Info:\n";
    for (Visitable* item : items) {
        item->accept(&visitor);
    }

    // Cleanup
    for (Visitable* item : items) {
        delete item;
    }

    return 0;
}
