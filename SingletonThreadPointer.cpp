using namespace std;


class Singleton {
    private:
        static Singleton* p;
        static mutex m_mtx;
        Singleton(){
            cout<<"Constructor"<<endl;
        }
        ~Singleton(){
            cout<<"Destructor"<<endl;
        }
        Singleton(const Singleton&) = delete;
        Singleton operator=(const Singleton&) = delete;
        Singleton(Singleton&&) = delete;
        Singleton operator=(Singleton&&) = delete;
    public:
        static Singleton* getInstance(){
            lock_guard<mutex> lock(m_mtx);
            if(p == NULL){
                p = new Singleton();
            }
            return p;
        }

        static void destroyInstance(){
            lock_guard<mutex> lock(m_mtx);
            delete p;
            p = NULL;
        }

        void myFunc(){
            cout<<"Hey there"<<endl;
        }

};

Singleton* Singleton::p = NULL;
mutex Singleton::m_mtx;

void threadFunction(){
     Singleton* p = Singleton::getInstance();
     p->myFunc();
     p->destroyInstance();
}

int main(){
     vector<thread> threads;
     for(int i = 0;i<10;i++){
         threads.emplace_back(threadFunction);
         //threads.push_back((thread)doSomething);
     }

     for(auto& th : threads){
         if(th.joinable()){
             th.join();
         }
     }
}