#include <iostream>
#include <unordered_map>
#include <functional>

using namespace std;

class Light {
public:
    void on(){
        cout << "Lights turned on" << endl;
    }
    
    void off(){
        cout << "Lights turned off" << endl;
    }
};

typedef function<void()> FunctionPointer;

class RemoteControl {
public:
    unordered_map<int, FunctionPointer> mp;
    
    void program(int slotNumber, FunctionPointer fPtr){
        mp[slotNumber] = fPtr;
    }
    
    void execute(int slotNumber){
        if (mp.find(slotNumber) != mp.end()) {
            mp[slotNumber]();  // Execute the stored function
        } else {
            cout << "No function programmed at slot " << slotNumber << endl;
        }
    }
};

int main() {
    Light* l = new Light();
    RemoteControl* rc = new RemoteControl();
    
    // Using a lambda to correctly bind the member function
    rc->program(1, [l]() { l->on(); });

    // Execute the programmed function
    rc->execute(1);

    delete l;
    delete rc;
    
    return 0;
}


// undo functionality implemented 

using namespace std;

class Light {
public:
    void on(){
        cout<<"Lights turned on"<<endl;
    }
    
    void off(){
        cout<<"Lights turned off"<<endl;
    }
};

class Command {
public:
    virtual void execute() = 0;
    virtual void undo() = 0;
};

class LightsOnCommand : public Command {
private:
    Light* light;
public:
    LightsOnCommand (Light* light) {
        this->light = light;
    }
    void execute() {
        light->on();
    }
    void undo(){
        light->off();
    }
};

class LightsOffCommand : public Command {
private:
    Light* light;
public:
    LightsOffCommand (Light* light) {
        this->light = light;
    }
    void execute() {
        light->off();
    }
    void undo(){
        light->on();
    }
};

class RemoteControl {
private:
    unordered_map<int, Command*> mp;
    Command* lastCmd;
public:
    RemoteControl(){
        lastCmd = NULL;
    }
    void program(int slotNumber, Command* cmd){
        mp[slotNumber] = cmd;
    }
    void pressButton(int slotNumber){
        mp[slotNumber]->execute();
        lastCmd = mp[slotNumber];
    }
    void pressUndoButton() {
        lastCmd->undo();
    }
};

int main() {
    Light* light = new Light();
    LightsOnCommand* onCmd = new LightsOnCommand(light);
    LightsOffCommand* offCmd = new LightsOffCommand(light);
    RemoteControl* rc = new RemoteControl();
    rc->program(1, onCmd);
    rc->program(2, offCmd);
    rc->pressButton(1);
    rc->pressUndoButton();
    rc->pressButton(2);
    rc->pressUndoButton();
    return 0;
}