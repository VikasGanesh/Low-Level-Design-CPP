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

//adapter class
class GooseAdapter : public Quackable {
private:
    Goose* goose;
public:
    GooseAdapter(Goose* goose){
        this->goose = goose;
    }
    void quack() {
        goose->honk();
    }
};

//target is it Quackable;




int main() {
    std::cout << "Hello World!\n";
}
