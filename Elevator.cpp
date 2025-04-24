#include <iostream>
#include <vector>
#include <queue>
#include <set>
#include <limits>
#include <cmath>
#include <algorithm>
using namespace std;

enum class Direction {
    UP,
    DOWN
};

enum class ElevatorState {
    IDLE,
    UP,
    DOWN
};

enum class DoorState {
    OPEN,
    CLOSE
};

class Button {
protected:
    bool status;
public:
    Button() : status(false) {}
    virtual bool isPressed() = 0;
    virtual void pressDown() {
        status = true;
    }
};

class HallButton : public Button {
private:
    Direction buttonSign;
public:
    HallButton(Direction buttonSign) : buttonSign(buttonSign) {}
    bool isPressed() override {
        return status;
    }
};

class ElevatorButton : public Button  {
private:
    int destinationFloorNumber;
public: 
    ElevatorButton(int floor) : destinationFloorNumber(floor) {}
    bool isPressed() override {
        return status;
    }
    int getFloor() const { return destinationFloorNumber; }
};

class Display {
public: 
    void showElevatorDisplay(int floor, Direction direction) {
        cout << "Elevator at floor " << floor << ", direction: " << (direction == Direction::UP ? "UP" : "DOWN") << endl;
    }
};

class Panel {
protected:
    int id;
public:
    Panel(int id) : id(id) {}
};

class ElevatorPanel : public Panel {
private:
    vector<ElevatorButton*> floorButtons;
public:
    ElevatorPanel(int id, int totalFloors) : Panel(id) {
        for (int i = 0; i < totalFloors; ++i) {
            floorButtons.push_back(new ElevatorButton(i));
        }
    }
    void pressButton(int floor) {
        floorButtons[floor]->pressDown();
    }
    vector<int> getPressedFloors() {
        vector<int> res;
        for (auto btn : floorButtons) {
            if (btn->isPressed()) res.push_back(btn->getFloor());
        }
        return res;
    }
};

class Door {
private: 
    DoorState state = DoorState::CLOSE;
public:
    bool isOpen() { return state == DoorState::OPEN; }
    void open() { state = DoorState::OPEN; }
    void close() { state = DoorState::CLOSE; }
};

class ElevatorCar {
private:
    ElevatorPanel* panel;
    Display display;
    Door door;
    int currentFloor;
    ElevatorState state;
    queue<int> floorQueue;
    int id;
    Direction currentDirection;
public:
    ElevatorCar(int id, int totalFloors) : id(id) {
        panel = new ElevatorPanel(id, totalFloors);
        currentFloor = 0;
        state = ElevatorState::IDLE;
        currentDirection = Direction::UP;
    }

    void requestFloor(int floor) {
        panel->pressButton(floor);
        floorQueue.push(floor);
    }

    void step() {
        // Move based on the current direction (LOOK strategy)
        if (!floorQueue.empty()) {
            int dest = floorQueue.front();
            
            if (currentDirection == Direction::UP && currentFloor < dest) {
                currentFloor++;
                state = ElevatorState::UP;
            } else if (currentDirection == Direction::DOWN && currentFloor > dest) {
                currentFloor--;
                state = ElevatorState::DOWN;
            } else {
                // Door opens and services the request
                door.open();
                cout << "Elevator " << id << " stopping at floor " << currentFloor << endl;
                floorQueue.pop();
                door.close();
                state = floorQueue.empty() ? ElevatorState::IDLE : state;
            }
        } else {
            state = ElevatorState::IDLE;
        }

        // After moving in current direction, check if end is reached
        if (currentFloor == 0 || currentFloor == 9) {
            currentDirection = (currentDirection == Direction::UP) ? Direction::DOWN : Direction::UP;
        }

        display.showElevatorDisplay(currentFloor, currentDirection);
    }

    int getCurrentFloor() const { return currentFloor; }
    ElevatorState getState() const { return state; }
    int getId() const { return id; }
    int getPendingRequests() const { return floorQueue.size(); }
};

// Strategy Pattern
class ElevatorStrategy {
public:
    virtual ElevatorCar* findBestElevator(vector<ElevatorCar*>& elevators, int requestedFloor) = 0;
};

// Look Strategy
class LookStrategy : public ElevatorStrategy {
public:
    ElevatorCar* findBestElevator(vector<ElevatorCar*>& elevators, int requestedFloor) override {
        ElevatorCar* best = nullptr;
        int minDistance = numeric_limits<int>::max();

        // Look for the closest elevator in the current direction
        for (ElevatorCar* elevator : elevators) {
            int distance = abs(elevator->getCurrentFloor() - requestedFloor);
            if (elevator->getState() == ElevatorState::IDLE || elevator->getState() == ElevatorState::UP) {
                if (elevator->getCurrentFloor() <= requestedFloor && distance < minDistance) {
                    minDistance = distance;
                    best = elevator;
                }
            }
        }

        // If no best elevator found, switch to down direction
        if (!best) {
            for (ElevatorCar* elevator : elevators) {
                int distance = abs(elevator->getCurrentFloor() - requestedFloor);
                if (elevator->getState() == ElevatorState::DOWN && distance < minDistance) {
                    minDistance = distance;
                    best = elevator;
                }
            }
        }

        return best;
    }
};

// The building class and elevator system remain unchanged
class Building {
private:
    vector<ElevatorCar*> elevators;
public:
    Building(int elevatorCount, int floors) {
        for (int i = 0; i < elevatorCount; ++i) {
            elevators.push_back(new ElevatorCar(i, floors));
        }
    }
    vector<ElevatorCar*>& getElevators() { return elevators; }
};

class ElevatorSystem {
private:
    Building* building;
    ElevatorStrategy* strategy;
public:
    ElevatorSystem(Building* building, ElevatorStrategy* strategy)
        : building(building), strategy(strategy) {}

    void placeRequest(int floor) {
        ElevatorCar* best = strategy->findBestElevator(building->getElevators(), floor);
        if (best) {
            cout << "Request at floor " << floor << " assigned to Elevator " << best->getId() << endl;
            best->requestFloor(floor);
        }
    }

    void stepAll() {
        for (ElevatorCar* e : building->getElevators()) {
            e->step();
        }
    }
};

int main() {
    Building* building = new Building(2, 10);
    cout << "Elevator Count: " << building->getElevators().size() << endl;

    ElevatorStrategy* strategy = new LookStrategy();
    ElevatorSystem system(building, strategy);

    system.placeRequest(3);
    system.placeRequest(6);
    system.placeRequest(1);

    for (int i = 0; i < 15; ++i) {
        system.stepAll();
    }

    delete strategy;
    delete building;
    return 0;
}
