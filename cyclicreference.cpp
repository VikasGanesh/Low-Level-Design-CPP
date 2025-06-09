class B;

class A {
public:
    shared_ptr<B> b;
    A(){
        cout<<"A Constructor"<<endl;
    }
    ~A(){
        cout<<"A Destructor"<<endl;
    }
};

class B {
public:
    weak_ptr<A> a;
    B(){
        cout<<"B Constructor"<<endl;
    }
    ~B(){
        cout<<"B Destructor"<<endl;
    }
};

int main() {
    shared_ptr<A> a = make_shared<A>();
    shared_ptr<B> b = make_shared<B>();
    a->b = b;
    b->a = a;
    
    return 0;
}
