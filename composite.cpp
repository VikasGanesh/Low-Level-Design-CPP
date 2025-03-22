#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class MenuComponent {
public:
    virtual void add(MenuComponent* menuComponent) {
        throw runtime_error("Unsupported action");
    }
    virtual void remove(MenuComponent* menuComponent) {
        throw runtime_error("Unsupported action");
    }
    virtual MenuComponent* getChild(int idx) {
        throw runtime_error("Unsupported action");
    }
    virtual string getName() {
        throw runtime_error("Unsupported action");
    }
    virtual string getDescription() {
        throw runtime_error("Unsupported action");
    }
    virtual double getPrice() {
        throw runtime_error("Unsupported action");
    }
    virtual bool isVegetarian() {
        throw runtime_error("Unsupported action");
    }
    virtual void print() {
        throw runtime_error("Unsupported action");
    }
};

class MenuItem : public MenuComponent {
private:
    string name;
    string description;
    double price;
    bool isVeg;
public:
    MenuItem(string name, string description, bool isVeg, double price){
        this->name = name;
        this->description = description;
        this->price = price;
        this->isVeg = isVeg;
    }
    string getName() {
        return name;
    }
    string getDescription() {
        return description;
    }
    double getPrice() {
        return price;
    }
    bool isVegetarian() {
        return isVeg;
    }
    void print() {
        cout<<getName()<<" "<<(isVegetarian() ? "Veg":"Non Veg")<<" "<<endl;
        cout<<getDescription()<<" "<<getPrice()<<endl;
    }
};

class Menu : public MenuComponent {
private:
    string name;
    string description;
    vector<MenuComponent*> menuComponents;
public:
    Menu(string name, string description){
        this->name = name;
        this->description = description;
    }
    void add(MenuComponent* menuComponent) {
        menuComponents.push_back(menuComponent);
    }
    void remove(MenuComponent* menuComponent) {
        auto itr = find(menuComponents.begin(), menuComponents.end(), menuComponent);
        if(itr == menuComponents.end()){
            return;
        }
        menuComponents.erase(itr);
    }
    MenuComponent* getChild(int idx) {
        return menuComponents[idx];
    }
    string getName(){
        return name;
    }
    string getDescription() {
        return description;
    }
    void print() {
        cout<<getName()<<" "<<getDescription()<<endl;
        for(auto itr = menuComponents.begin(); itr != menuComponents.end(); itr++){
            (*itr)->print();
        }
    }
};

class Waitress {
private:
    MenuComponent* allMenus;
public:
    Waitress(MenuComponent* allMenus){
        this->allMenus = allMenus;
    }
    void printMenu() {
        allMenus->print();
    }
};

int main() {
    MenuComponent* pancakeHouseMenu = new Menu("PANCAKE HOUSE MENU", "Breakfast");
    MenuComponent* dinerMenu = new Menu("DINER MENU", "Lunch");
    MenuComponent* cafeMenu = new Menu("CAFE MENU", "Dinner");
    MenuComponent* dessertMenu = new Menu("DESSERT MENU", "Dessert of course!");
    MenuComponent* allMenu = new Menu("ALL MENUS", "All menus combined");
    allMenu->add(pancakeHouseMenu);
    allMenu->add(dinerMenu);
    allMenu->add(cafeMenu);
    dinerMenu->add(new MenuItem("Pasta", "Spaghetti with Marinara Sauce, and a slice of sourdough bread", true, 3.89));
    dinerMenu->add(dessertMenu);
    dinerMenu->add(new MenuItem("Apple Pie", "Apple pie with a flakey crust, topped with vanilla ice cream", true, 1.59));
    Waitress* waitress = new Waitress(allMenu);
    waitress->printMenu();
    return 0;
}
