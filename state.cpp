#include <iostream>
using namespace std;

class GumballMachine;  // Forward declaration

class State {  
public:
    virtual void insertQuarter() = 0;
    virtual void ejectQuarter() = 0;
    virtual void turnCrank() = 0;
    virtual void dispense() = 0;
    virtual ~State() {} 
};

// =============================
// NoQuarterState Class
// =============================
class NoQuarterState : public State {
private:
    GumballMachine* gumballMachine;
public:
    NoQuarterState(GumballMachine* gumballMachine);
    void insertQuarter() override;
    void ejectQuarter() override;
    void turnCrank() override;
    void dispense() override;
};

// =============================
// HasQuarterState Class
// =============================
class HasQuarterState : public State {
private:
    GumballMachine* gumballMachine;
public:
    HasQuarterState(GumballMachine* gumballMachine);
    void insertQuarter() override;
    void ejectQuarter() override;
    void turnCrank() override;
    void dispense() override;
};

// =============================
// SoldState Class
// =============================
class SoldState : public State {
private:
    GumballMachine* gumballMachine;
public:
    SoldState(GumballMachine* gumballMachine);
    void insertQuarter() override;
    void ejectQuarter() override;
    void turnCrank() override;
    void dispense() override;
};

// =============================
// SoldOutState Class
// =============================
class SoldOutState : public State {
private:
    GumballMachine* gumballMachine;
public:
    SoldOutState(GumballMachine* gumballMachine);
    void insertQuarter() override;
    void ejectQuarter() override;
    void turnCrank() override;
    void dispense() override;
};

// =============================
// GumballMachine Class
// =============================
class GumballMachine {
private:
    State* soldOutState;
    State* noQuarterState;
    State* hasQuarterState;
    State* soldState;
    State* state;
    int count;
public:
    GumballMachine(int count);
    void insertQuarter();
    void ejectQuarter();
    void turnCrank();
    void setState(State* state);
    void releaseBall();
    
    State* getHasQuarterState();
    State* getNoQuarterState();
    State* getSoldState();
    State* getSoldOutState();
    int getCount();
};

// =============================
// NoQuarterState Method Definitions
// =============================
NoQuarterState::NoQuarterState(GumballMachine* gumballMachine) {
    this->gumballMachine = gumballMachine;
}

void NoQuarterState::insertQuarter() {
    cout << "You inserted a quarter." << endl;
    gumballMachine->setState(gumballMachine->getHasQuarterState());
}

void NoQuarterState::ejectQuarter() {
    cout << "You haven't inserted a quarter." << endl;
}

void NoQuarterState::turnCrank() {
    cout << "You turned, but there's no quarter." << endl;
}

void NoQuarterState::dispense() {
    cout << "You need to pay first." << endl;
}

// =============================
// HasQuarterState Method Definitions
// =============================
HasQuarterState::HasQuarterState(GumballMachine* gumballMachine) {
    this->gumballMachine = gumballMachine;
}

void HasQuarterState::insertQuarter() {
    cout << "You can't insert another quarter." << endl;
}

void HasQuarterState::ejectQuarter() {
    cout << "Quarter returned." << endl;
    gumballMachine->setState(gumballMachine->getNoQuarterState());
}

void HasQuarterState::turnCrank() {
    cout << "You turned..." << endl;
    gumballMachine->setState(gumballMachine->getSoldState());
}

void HasQuarterState::dispense() {
    cout << "No gumball dispensed." << endl;
}

// =============================
// SoldState Method Definitions
// =============================
SoldState::SoldState(GumballMachine* gumballMachine) {
    this->gumballMachine = gumballMachine;
}

void SoldState::insertQuarter() {
    cout << "Please wait, we're already giving you a gumball." << endl;
}

void SoldState::ejectQuarter() {
    cout << "Sorry, you already turned the crank." << endl;
}

void SoldState::turnCrank() {
    cout << "Turning twice doesn't get you another gumball!" << endl;
}

void SoldState::dispense() {
    gumballMachine->releaseBall();
    if (gumballMachine->getCount() > 0) {
        gumballMachine->setState(gumballMachine->getNoQuarterState());
    } else {
        cout << "Oops, out of gumballs!" << endl;
        gumballMachine->setState(gumballMachine->getSoldOutState());
    }
}

// =============================
// SoldOutState Method Definitions
// =============================
SoldOutState::SoldOutState(GumballMachine* gumballMachine) {
    this->gumballMachine = gumballMachine;
}

void SoldOutState::insertQuarter() {
    cout << "You can't insert a quarter, the machine is sold out." << endl;
}

void SoldOutState::ejectQuarter() {
    cout << "You can't eject, you haven't inserted a quarter yet." << endl;
}

void SoldOutState::turnCrank() {
    cout << "You turned, but there are no gumballs." << endl;
}

void SoldOutState::dispense() {
    cout << "No gumball dispensed." << endl;
}

// =============================
// GumballMachine Method Definitions
// =============================
GumballMachine::GumballMachine(int count) {
    this->count = count;
    soldOutState = new SoldOutState(this);
    noQuarterState = new NoQuarterState(this);
    hasQuarterState = new HasQuarterState(this);
    soldState = new SoldState(this);
    
    state = (count > 0) ? noQuarterState : soldOutState;
}

void GumballMachine::insertQuarter() {
    state->insertQuarter();
}

void GumballMachine::ejectQuarter() {
    state->ejectQuarter();
}

void GumballMachine::turnCrank() {
    state->turnCrank();
    state->dispense();
}

void GumballMachine::setState(State* newState) {
    this->state = newState;
}

void GumballMachine::releaseBall() {
    cout << "A gumball comes rolling out the slot..." << endl;
    if (count > 0) {
        count -= 1;
    }
}

State* GumballMachine::getHasQuarterState() {
    return hasQuarterState;
}

State* GumballMachine::getNoQuarterState() {
    return noQuarterState;
}

State* GumballMachine::getSoldState() {
    return soldState;
}

State* GumballMachine::getSoldOutState() {
    return soldOutState;
}

int GumballMachine::getCount() {
    return count;
}

// =============================
// Main Function
// =============================
int main() {
    GumballMachine gumballMachine(3);

    gumballMachine.insertQuarter();
    gumballMachine.turnCrank();

    gumballMachine.insertQuarter();
    gumballMachine.ejectQuarter();
    gumballMachine.turnCrank();

    gumballMachine.insertQuarter();
    gumballMachine.turnCrank();

    gumballMachine.insertQuarter();
    gumballMachine.turnCrank();

    return 0;
}
