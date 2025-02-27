#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class Beverage {
protected:
    string description;
public:
    virtual string getDescription() = 0;
    virtual double cost() = 0;
};

class Espresso : public Beverage {
public:
    Espresso(){
        description = "Espresso";
    }
    string getDescription() {
        return description;
    }
    
    double cost(){
        return 1.99;
    }
};

class HouseBlend : public Beverage {
public:
    HouseBlend(){
        description = "House Blend";
    }
    string getDescription() {
        return description;
    }

    double cost(){
        return 0.89;
    }
};

class BeverageDecorator : public Beverage {
protected:
    Beverage* beverage;
};


class Mocha : public BeverageDecorator {
public:
    Mocha (Beverage* beverage) {
        this->beverage = beverage;
    } 
    string getDescription() {
        return beverage->getDescription() + ", Mocha";
    }

    double cost(){
        return beverage->cost() + 0.20;
    }  
};

class Caramel : public BeverageDecorator {
public:
    Caramel (Beverage* beverage) {
        this->beverage = beverage;
    } 
    string getDescription() {
        return beverage->getDescription() + ", Caramel";
    }

    double cost(){
        return beverage->cost() + 0.21;
    }  
};

int main() {
    Beverage* beverage = new Espresso();
    cout<<beverage->getDescription()<<" "<<beverage->cost()<<endl;

    Beverage* beverage2 = new HouseBlend();
    cout<<beverage2->getDescription()<<" "<<beverage2->cost()<<endl;
    
    beverage2 = new Mocha(beverage2);
    cout<<beverage2->getDescription()<<" "<<beverage2->cost()<<endl;

    beverage2 = new Mocha(beverage2);
    cout<<beverage2->getDescription()<<" "<<beverage2->cost()<<endl;
    
    beverage2 = new Caramel(beverage2);
    cout<<beverage2->getDescription()<<" "<<beverage2->cost()<<endl;

    
}