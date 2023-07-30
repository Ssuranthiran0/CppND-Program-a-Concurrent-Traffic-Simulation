#include <iostream>
#include <ctime> // clock
#include <random>
#include <memory>
#include <queue>
#include <algorithm>
#include "TrafficLight.h"
#include "TrafficObject.h"

// 90% of this project (parts that are different from starter code) came from the lesson content, especially the messagequeue


/* Implementation of class "MessageQueue" */


template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a :: DONE
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() DONE
    // to wait for and receive new messages and pull them from the queue using move semantics. DONE
    // The received object should then be returned by the receive function. DONE
    // One quick thing. This was incredibly simple, because the message queue in the lesson content was essentially identical to this
    // maybe that was the point

    std::unique_lock<std::mutex> uLock(_mutex); // unique lock to prevent deadlock. kinda nice honestly. like a unique ptr for mutex (dont unlock/lock), let it do it
    _cond.wait(uLock, [this] {
        return !_queue.empty();
    });

    T msg = std::move(_queue.back()); // last message (in queue) so first message sent in the queue
    _queue.pop_back();

    return msg;

}

template <typename T>
void MessageQueue<T>::send(T &&msg) // move into
{
    // FP.4a :: DONE
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> DONE
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.  DONE
    std::lock_guard<std::mutex> uLock(_mutex);
    _queue.push_back(std::move(msg)); // move into
    _cond.notify_one(); // tell it that something have been moved into queue
}


/* Implementation of class "TrafficLight" */

 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
    _messageQueue = std::make_shared<MessageQueue<TrafficLightPhase>> (); // create a message queue
}
TrafficLight::~TrafficLight(){
    for ( std::thread &t : _threads){
        t.join(); // join all threads at the very end, or more accurately, make all threads join this one.
    }
    // used to have a vector<std::thread> _threads
    // no need to deallocate _messageQueue ptr since it is smart ptr
}

void TrafficLight::waitForGreen()
{
    // FP.5b :: DONE
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop  DONE
    // runs and repeatedly calls the receive function on the message queue. DONE
    // Once it receives TrafficLightPhase::green, the method returns. DONE

    while(true){
        if(_messageQueue->receive() == TrafficLightPhase::green){
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
    // FP.2b :: DONE
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
    _threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this)); // pass a pointer to the current traffic light and a ref to the method being called
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a :: DONE
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles DONE
    // and toggles the current phase of the traffic light between red and green and sends an update method DONE
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. DONE
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles. DONE

    // scroll down (left the big comment for reference (old code))

    /*int elapsed{0}; // ms
    int changeRate{0}; // ik it auto inits to 0 but idk
    while (true){ // NOTE: just adding 1 to elapsed ms, does not account for calculation times, but the rng makes it essentially irrelevant in this scenario
        if(elapsed <= 0){
            srand((unsigned) time(0)); // set seed for random num generator
            elapsed = rand() % 2000 + 4000; // random(0,2000) + 4000 = (0-2) seconds + (4) seconds = (4-6) seconds
            // toggle
            if(_currentPhase = TrafficLightPhase::red){
                _currentPhase = TrafficLightPhase::green;
            }else{
                _currentPhase = TrafficLightPhase::red;
            }
            // send a message to the queue
            _messageQueue->send(std::move(_currentPhase));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1)); // sleep for 1 ms between cycles
        elapsed -= 1;
    }
    while(true){
        std::this_thread::sleep_for(std::chrono::milliseconds(4000));
        if(_currentPhase = TrafficLightPhase::red){
            _currentPhase = TrafficLightPhase::green;
        }else{
            _currentPhase = TrafficLightPhase::red;
        }
    }*/
    // kinda dumb but it works so...
    while(true){
        srand(time(0)); // set seed for random num generator
        std::this_thread::sleep_for(std::chrono::milliseconds(4000+ (rand()%2000)));
        _currentPhase = TrafficLightPhase::green;
        std::this_thread::sleep_for(std::chrono::milliseconds(4000+ (rand()%2000)));
        _currentPhase = TrafficLightPhase::red;
    }

}

