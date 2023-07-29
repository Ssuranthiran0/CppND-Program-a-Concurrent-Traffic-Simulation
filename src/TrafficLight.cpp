#include <iostream>
#include <ctime> // clock
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop  DONE
    // runs and repeatedly calls the receive function on the message queue. DONE
    // Once it receives TrafficLightPhase::green, the method returns. DONE

    TrafficLightPhase message;
    while(true){
        message = _messageQueue.receive();
        if(message == TrafficLightPhase::green){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    _threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles DONE
    // and toggles the current phase of the traffic light between red and green and sends an update method DONE
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. DONE
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. DONE

    int elapsed{0}; // ms
    int changeRate{0}; // ik it auto inits to 0 but idk

    while (true){ // NOTE: just adding 1 to elapsed ms, does not account for calculation times, but the rng makes it essentially irrelevant in this scenario
        if(elapsed <= 0){ // update
            srand((unsigned) time(0)); // set seed for random num generator
            elapsed = rand() % 2000 + 4000; // random(0,2000) + 4000 = (0-2) seconds + (4) seconds = (4-6) seconds
            if(_currentPhase = TrafficLightPhase::red){

                _currentPhase = TrafficLightPhase::green;
            }else{
                _currentPhase = TrafficLightPhase::red;
            }
            _messageQueue.send(std::move(_currentPhase));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // sleep for 1 ms between cycles
        elapsed -= 1;
    }
}

