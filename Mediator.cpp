#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

using namespace std;

// Forward declaration
class SmartDevice;

// Smart House Mediator with dynamic event handling
class SmartHouseMediator {
private:
    unordered_map<string, vector<function<void()>>> eventHandlers;

public:
    // Register an event with a callback function
    void registerEvent(const string& event, function<void()> callback) {
        eventHandlers[event].push_back(callback);
    }

    // Notify all registered callbacks for an event
    void notify(const string& event) {
        if (eventHandlers.find(event) != eventHandlers.end()) {
            for (auto& callback : eventHandlers[event]) {
                callback();  // Execute the associated action
            }
        }
    }
};

// Base class for all smart devices
class SmartDevice {
protected:
    SmartHouseMediator* mediator;
    string name;

public:
    SmartDevice(SmartHouseMediator* mediator, string name) : mediator(mediator), name(name) {}

    virtual void doAction() = 0;
};

// Concrete Smart Devices
class Alarm : public SmartDevice {
public:
    using SmartDevice::SmartDevice;

    void doAction() override {
        cout << name << " ringing! ⏰\n";
        mediator->notify("AlarmRing");
    }
};

class CoffeeMachine : public SmartDevice {
public:
    using SmartDevice::SmartDevice;

    void doAction() override {
        cout << name << " is brewing coffee! ☕\n";
    }
};

class Sprinkler : public SmartDevice {
public:
    using SmartDevice::SmartDevice;

    void doAction() override {
        cout << name << " stopping... 🚿\n";
    }
};

class Shower : public SmartDevice {
public:
    using SmartDevice::SmartDevice;

    void doAction() override {
        cout << name << " starting... 🚿\n";
    }
};

// Client Code
int main() {
    SmartHouseMediator house;

    Alarm alarm(&house, "Alarm");
    CoffeeMachine coffee(&house, "CoffeeMachine");
    Sprinkler sprinkler(&house, "Sprinkler");
    Shower shower(&house, "Shower");

    // Register events dynamically
    house.registerEvent("AlarmRing", [&]() { coffee.doAction(); });
    house.registerEvent("AlarmRing", [&]() { sprinkler.doAction(); });

    // Trigger alarm
    alarm.doAction();  // Alarm rings → Coffee brews → Sprinkler stops

    return 0;
}
