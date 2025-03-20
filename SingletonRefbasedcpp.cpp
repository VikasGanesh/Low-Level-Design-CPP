#include <iostream>
#include <bits/stdc++.h>
#include <thread>
#include <mutex>


using namespace std;

class Singleton {
    private:
      Singleton() {
          cout<<"Constructor"<<endl;
      }
      // Delete copy constructor
      Singleton(Singleton&) = delete;
      // Delete copy assignment operator
      Singleton operator=(Singleton&) = delete;
      // Delete move constructor
      Singleton(Singleton&&) = delete;
      // Delete move assignment operator
      Singleton operator=(Singleton&&) = delete;
      ~Singleton() {}
   public:
      static Singleton& getInstance() {
          static Singleton instance;
          return instance;
      }
      void doSomething(){
          cout<<"Hey there"<<endl;
      }
};

void threadFunction(){
     Singleton& ins = Singleton::getInstance();
     ins.doSomething();
}

int main() {
    vector<thread> threads;
    for(int i = 0;i<10;i++){
        threads.emplace_back(threadFunction);
    }

    for(int i = 0;i<10;i++){
        if(threads[i].joinable()){
            threads[i].join();
        }
    }
}
~         