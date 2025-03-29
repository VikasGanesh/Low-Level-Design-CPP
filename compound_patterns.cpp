#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

class QuackObservable;

// Observer Interface
class Observer {
public:
    virtual void update(QuackObservable* duck) = 0;
};

// Iterator Template
template <typename T>
class Iterator {
public:
    virtual T getNext() = 0;
    virtual bool hasNext() = 0;
};

// Observer Iterator
class ObserverIterator : public Iterator<Observer*> {
private:
    int index;
    vector<Observer*>& observers;
public:
    ObserverIterator(vector<Observer*>& observers) : observers(observers), index(0) {}
    Observer* getNext() {
        return hasNext() ? observers[index++] : nullptr;
    }
    bool hasNext() {
        return index < observers.size();
    }
};

// Observable Interface
class QuackObservable {
public:
    virtual void registerObserver(Observer* observer) = 0;
    virtual void notifyObservers() = 0;
};

// Observable Implementation
class Observable : public QuackObservable {
private:
    vector<Observer*> observers;
    QuackObservable* duck;
    unordered_set<Observer*> observerSet;
public:
    Observable(QuackObservable* duck) : duck(duck) {}
    
    void registerObserver(Observer* observer) {
        if (observerSet.find(observer) == observerSet.end()) { // Avoid duplicate registration
            observers.push_back(observer);
            observerSet.insert(observer);
        }
    }
    
    void notifyObservers() {
        ObserverIterator iterator(observers);
        while (iterator.hasNext()) {
            iterator.getNext()->update(duck);
        }
    }
};

// Quackable Interface
class Quackable : public QuackObservable {
public:
    virtual void quack() = 0;
};

// Concrete Ducks
class MallardDuck : public Quackable {
private:
    Observable observable;
public:
    MallardDuck() : observable(this) {}
    void quack() {
        cout << "Quack" << endl;
        notifyObservers();
    }
    void registerObserver(Observer* observer) {
        observable.registerObserver(observer);
    }
    void notifyObservers() {
        observable.notifyObservers();
    }
};

class RedheadDuck : public Quackable {
private:
    Observable observable;
public:
    RedheadDuck() : observable(this) {}
    void quack() {
        cout << "Quack" << endl;
        notifyObservers();
    }
    void registerObserver(Observer* observer) {
        observable.registerObserver(observer);
    }
    void notifyObservers() {
        observable.notifyObservers();
    }
};

class DuckCall : public Quackable {
private:
    Observable observable;
public:
    DuckCall() : observable(this) {}
    void quack() {
        cout << "Kwak" << endl;
        notifyObservers();
    }
    void registerObserver(Observer* observer) {
        observable.registerObserver(observer);
    }
    void notifyObservers() {
        observable.notifyObservers();
    }
};

class RubberDuck : public Quackable {
private:
    Observable observable;
public:
    RubberDuck() : observable(this) {}
    void quack() {
        cout << "Squeak" << endl;
        notifyObservers();
    }
    void registerObserver(Observer* observer) {
        observable.registerObserver(observer);
    }
    void notifyObservers() {
        observable.notifyObservers();
    }
};

// Goose Adapter
class Goose {
public:
    void honk() {
        cout << "Honk" << endl;
    }
};

class GooseAdapter : public Quackable {
private:
    Goose* goose;
    Observable observable;
public:
    GooseAdapter(Goose* goose) : goose(goose), observable(this) {}
    void quack() {
        goose->honk();
        notifyObservers();
    }
    void registerObserver(Observer* observer) {
        observable.registerObserver(observer);
    }
    void notifyObservers() {
        observable.notifyObservers();
    }
};

// Quackologist Observer
class Quackologist : public Observer {
public:
    void update(QuackObservable* duck) {
        cout << "Quackologist: Duck just quacked." << endl;
    }
};

// Quack Counter Decorator
class QuackCounter : public Quackable {
private:
    Quackable* duck;
    static int numberOfQuacks;
public:
    QuackCounter(Quackable* duck) : duck(duck) {}
    void quack() {
        numberOfQuacks++;
        duck->quack();
    }
    static int getQuacks() {
        return numberOfQuacks;
    }
    void registerObserver(Observer* observer) {
        duck->registerObserver(observer);
    }
    void notifyObservers() {
        duck->notifyObservers();
    }
};

int QuackCounter::numberOfQuacks = 0;

// Factory Pattern
class AbstractDuckFactory {
public:
    virtual Quackable* createMallardDuck() = 0;
    virtual Quackable* createRedheadDuck() = 0;
    virtual Quackable* createDuckCall() = 0;
    virtual Quackable* createRubberDuck() = 0;
};

class CountingDuckFactory : public AbstractDuckFactory {
public:
    Quackable* createMallardDuck() { return new QuackCounter(new MallardDuck()); }
    Quackable* createRedheadDuck() { return new QuackCounter(new RedheadDuck()); }
    Quackable* createDuckCall() { return new QuackCounter(new DuckCall()); }
    Quackable* createRubberDuck() { return new QuackCounter(new RubberDuck()); }
};

// Flock Composite Pattern
class Flock : public Quackable {
private:
    vector<Quackable*> ducks;
public:
    void add(Quackable* duck) { ducks.push_back(duck); }
    void quack() {
        for (Quackable* duck : ducks) {
            duck->quack();
        }
    }
    void registerObserver(Observer* observer) {
        for (Quackable* duck : ducks) {
            duck->registerObserver(observer);
        }
    }
    void notifyObservers() {}
};

// Duck Simulator
class DuckSimulator {
public:
    void simulate(AbstractDuckFactory* factory) {
        Quackable* mallardDuck = factory->createMallardDuck();
        Quackable* redheadDuck = factory->createRedheadDuck();
        Quackable* duckCall = factory->createDuckCall();
        Quackable* rubberDuck = factory->createRubberDuck();
        Quackable* goose = new GooseAdapter(new Goose());
        
        Quackologist observer;
        mallardDuck->registerObserver(&observer);
        redheadDuck->registerObserver(&observer);
        duckCall->registerObserver(&observer);
        rubberDuck->registerObserver(&observer);
        
        Flock flock;
        flock.add(mallardDuck);
        flock.add(redheadDuck);
        flock.add(duckCall);
        flock.add(rubberDuck);
        
        flock.quack();
        cout << "The ducks quacked " << QuackCounter::getQuacks() << " times." << endl;
    }
};

int main() {
    DuckSimulator simulator;
    AbstractDuckFactory* factory = new CountingDuckFactory();
    simulator.simulate(factory);
    delete factory;
    return 0;
}
