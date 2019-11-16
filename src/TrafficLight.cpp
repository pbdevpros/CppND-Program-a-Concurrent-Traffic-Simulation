#include <iostream>
#include <chrono>
#include <random>
#include <cmath>
#include "TrafficLight.h"

double getRandomNumber(double min, double max);

template <typename T>
T MessageQueue<T>::receive()
{
    std::unique_lock<std::mutex> rLock(_mtx);
    _condvar.wait(rLock);
    T msg = std::move(_queue.front());
    _queue.pop_front();
    return msg;
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    std::lock_guard<std::mutex> sLock(_mtx);
    _queue.push_back(msg);
    _condvar.notify_one();    
}

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

TrafficLight::~TrafficLight()
{
    _currentPhase = red;
}

void TrafficLight::waitForGreen()
{
    TrafficLightPhase vPhase;
    while ( true ) {
        vPhase = trafficQue.receive();
        if (vPhase == green ){
            return ;
        }
    }
}

TrafficLight::TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    std::thread t(&TrafficLight::cycleThroughPhases, this); 
    threads.push_back(std::move(t));
}

void TrafficLight::cycleThroughPhases()
{
    while (true) {
        std::chrono::milliseconds runTime (0);
        auto startTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()); // time now in milliseconds 
        auto lightTimer = getRandomNumber(4.0, 6.0) * 1000; // milliseconds
        while ( runTime.count() <  lightTimer ) {
            runTime = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()) - startTime;
        }
        _currentPhase = ( _currentPhase == red ) ? green : red; 
        TrafficLightPhase light = _currentPhase;       
        trafficQue.send(std::move(light));
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

double getRandomNumber(double min, double max )
{
    // standard method of random number generator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist( min, max);
    return dist(mt);
}

long decimalValue(double num)
{
    int factor = 100;
    int decimal =  (int) ( num * factor ) %factor ;
    double value = std::round(num);
    long dec = decimal / factor ;
    return (value + dec );
}