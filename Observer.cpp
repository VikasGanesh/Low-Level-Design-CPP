#include <bits/stdc++.h>
#include <iostream>

using namespace std;

class Observer {
public:
    virtual void update() = 0;
};

class Subject {
public:
    virtual void subscribe(Observer* ob) = 0;
    virtual void unsubscribe(Observer* ob) = 0;
    virtual void update() = 0;
};

class WeatherData : public Subject {
private:
    double temperature;
    double windSpeed;
    double humidity;
    double pressure;
    vector<Observer*> observers;
public:
    WeatherData(){
        this->temperature = 0;
        this->windSpeed = 0;
        this->humidity = 0;
        this->pressure = 0;
    }
    
    void subscribe(Observer* ob){
        observers.push_back(ob);
    }
    
    void unsubscribe(Observer* ob){
        auto itr = find(observers.begin(), observers.end(), ob);
        if(itr == observers.end()){
            return;
        }
        observers.erase(itr);
    }
    
    double getTemperature() {
        return temperature;
    }

    double getWindSpeed() {
        return windSpeed;
    }

    double getHumidity() {
        return humidity;
    }

    double getPressure() {
        return pressure;
    }
    
    void updateTheData(double temperature, double windSpeed, double humidity, double pressure){
        this->temperature = temperature;
        this->windSpeed = windSpeed;
        this->humidity = humidity;
        this->pressure = pressure;
        update();
    }
    
    void update() {
        for(auto itr = observers.begin(); itr != observers.end(); itr++){
            (*itr)->update();
        }
    }
};


class DisplayElement {
public:
    virtual void display() = 0;
};

class WeatherSummaryDisplay : public DisplayElement, Observer {
private:
    WeatherData* wd;
public:
    WeatherSummaryDisplay(WeatherData* wd){
        this->wd = wd;
        wd->subscribe(this);
    }
    
    void update(){
        display();
    }
    
    void display() {
        cout<<"The temperature is "<<wd->getTemperature()<<" Celcius and wind speed being "<<wd->getWindSpeed()<<endl;
    }
};

int main() {
    WeatherData* wd = new WeatherData();
    wd->updateTheData(11, 21.1, 54.6, 44);
    WeatherSummaryDisplay*  dis = new WeatherSummaryDisplay(wd);
    wd->updateTheData(11, 21.1, 54.6, 44);
}