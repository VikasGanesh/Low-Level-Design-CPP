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

/*
This quack counting is great. We’re learning
things we never knew about the little quackers.
But we’re finding that too many quacks aren’t
being counted. 


we will use factory design pattern to create ducks now;
*/

class AbstractDuckFactory {
public:
    virtual Quackable* createMallardDuck() = 0;
    virtual Quackable* createRedheadDuck() = 0;
    virtual Quackable* createDuckCall() = 0;
    virtual Quackable* createRubberDuck() = 0;
};

class DuckFactory : public AbstractDuckFactory {
public:
    Quackable* createMallardDuck() {
        return new MallardDuck();
    }
    Quackable* createRedheadDuck() {
        return new RedheadDuck();
    }
    Quackable* createDuckCall() {
        return new DuckCall();
    }
    Quackable* createRubberDuck() {
        return new RubberDuck();
    }
};

class CountingDuckFactory : public AbstractDuckFactory {
public:
    Quackable* createMallardDuck() {
        return new QuackCounter(new MallardDuck());
    }
    Quackable* createRedheadDuck() {
        return new QuackCounter(new RedheadDuck());
    }
    Quackable* createDuckCall() {
        return new QuackCounter(new DuckCall());
    }
    Quackable* createRubberDuck() {
        return new QuackCounter(new RubberDuck());
    }
};


class DuckSimulator {
public:
    void simulate(AbstractDuckFactory* countingDuckFactory){
        Quackable* mallardDuck = countingDuckFactory->createMallardDuck();
        Quackable* readHead = countingDuckFactory->createRedheadDuck();
        Quackable* duckCall = countingDuckFactory->createDuckCall();
        Quackable* rubberDuck = countingDuckFactory->createRubberDuck();
        Quackable* gooseAdapter = new GooseAdapter(new Goose());

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
    AbstractDuckFactory* countingDuckFactory = new CountingDuckFactory();
    simulator.simulate(countingDuckFactory);
    return 0;
}
