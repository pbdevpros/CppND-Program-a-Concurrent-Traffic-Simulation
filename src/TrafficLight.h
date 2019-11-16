#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;

template <class T>
class MessageQueue
{
public:
    void send(T &&msg);
    T receive();
    std::deque<T> _queue;

private:
    std::condition_variable _condvar;
    std::mutex _mtx;
};

class TrafficLight : TrafficObject
{
public:
    TrafficLight();
    ~TrafficLight();

    typedef enum {
        red = 0,
        green,
    } TrafficLightPhase;


    void waitForGreen();
    void simulate();
    TrafficLightPhase getCurrentPhase();

private:
    void cycleThroughPhases();
    TrafficLightPhase _currentPhase;
    MessageQueue<TrafficLightPhase> trafficQue;
    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif