class Quackable {
public:
    virtual void quack() = 0;
};

class MallardDuck : public Quackable  {
public:
    void quack() {
        cout<<"Quack"<<endl;
    }
};

class RedheadDuck : public Quackable  {
public:
    void quack() {
        cout<<"Quack"<<endl;
    }
};

class DuckCall : public Quackable  {
public:
    void quack() {
        cout<<"Kwak"<<endl;
    }
};

class RubberDuck : public Quackable  {
public:
    void quack() {
        cout<<"Squeak"<<endl;
    }
};

//Adapter design pattern 

//Adaptee
class Goose{
public:
    void honk() {
        cout<<"Honk"<<endl;
    }
};
//target is it Quackable;
//adapter class
class GooseAdapter : public Quackable {
private:
    //Adaptee
    Goose* goose;
public:
    GooseAdapter(Goose* goose){
        this->goose = goose;
    }
    void quack() {
        goose->honk();
    }
};


/*
We’re going to make those Quackologists happy and give
them some quack counts. we will use decorator design pattern 
Let’s create a decorator that gives the ducks some new
behavior (the behavior of counting) by wrapping them with a
decorator object. We won’t have to change the Duck code at all.
*/

class QuackCounter : public Quackable {
private:
    Quackable* duck;
    static int numberOfQuacks;
public:
    QuackCounter(Quackable* duck){
        this->duck = duck;
    }

    void quack() {
        numberOfQuacks++;
        duck->quack();
    }
    
    static int getgetQuacks(){
        return numberOfQuacks;
    }
};

int QuackCounter::numberOfQuacks = 0;

class DuckSimulator {
public:
    void simulate(){
        Quackable* mallardDuck = new QuackCounter(new MallardDuck());
        Quackable* readHead = new QuackCounter(new RedheadDuck());
        Quackable* duckCall = new QuackCounter(new DuckCall());
        Quackable* rubberDuck = new QuackCounter(new RubberDuck());
        Quackable* gooseAdapter = new QuackCounter(new GooseAdapter(new Goose()));

        mallardDuck->quack();
        readHead->quack();
        rubberDuck->quack();
        duckCall->quack();
        gooseAdapter->quack();
        
        cout<<"The ducks quacked "<<QuackCounter::getgetQuacks()<<endl;
    }
};

int main() {
    DuckSimulator simulator;
    simulator.simulate();
    return 0;
}
