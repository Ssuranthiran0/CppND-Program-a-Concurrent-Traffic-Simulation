#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class „MessageQueue“ which has the public methods send and receive. DONE 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type.  DONE
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase.  DOME
// Also, there should be an std::condition_variable as well as an std::mutex as private members. DONE

template <class T>
class MessageQueue
{
public:
    T receive();
    void send(T &&msg);
private:
    std::deque<TrafficLightPhase> _queue;
    std::mutex _mutex;
    std::condition_variable _condition;
    
};

// FP.1 : Define a class „TrafficLight“ which is a child class of TrafficObject. DONE
// The class shall have the public methods „void waitForGreen()“ and „void simulate()“  DONE
// as well as „TrafficLightPhase getCurrentPhase()“, where TrafficLightPhase is an enum that  DONE
// can be either „red“ or „green“. Also, add the private method „void cycleThroughPhases()“.  DONE
// Furthermore, there shall be the private member _currentPhase which can take „red“ or „green“ as its value. DONE

enum TrafficLightPhase{red, green};

class TrafficLight
{
public:
    // constructor / desctructor
    TrafficLight();
    ~TrafficLight();
    // getters / setters
    
    // typical behaviour methods
    void waitForGreen();
    void simulate();

    TrafficLightPhase getCurrentPhase();

private:
    // typical behaviour methods
    void cycleThroughPhases();

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase DONE
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling DONE
    // send in conjunction with move semantics. DONE
    MessageQueue<TrafficLightPhase> _messageQueue;
    TrafficLightPhase _currentPhase;
    std::condition_variable _condition;
    std::vector<std::thread> _threads;
    std::mutex _mutex;
};

#endif