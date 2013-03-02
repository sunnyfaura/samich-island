#include "cinder/Cinder.h"
using namespace ci;
using namespace ci::app;

#ifndef STATE_HPP__
#define STATE_HPP__

static const int INIT = 0;
static const int PLAY = 1;
    
class State {
public:
    State(){ current = -1; }
    ~State(){}

    //set the state to be committed in next call to commitState()
    void setNextState( int newState ) {
        if ( newState != current ) {
            if ( validateState( newState ) ) {
                next = newState;
            }
        }
    }
        
    //return true if there has been a change in state
    bool commitState() {
        if ( next != current ) {
            previous = current;
            current = next;
            console() << "State.commitState: " << current << std::endl;
            return true;
        } else {
            return false;
        }
    }
        
    //return the current state
    int getCurrentState() { return current; }
        
    int getPreviousState() { return previous; }
        
    //return true if the new state is valid
    bool validateState( int newState ){
        return newState == INIT || 
                newState == PLAY;
    }
        
private:
    int current;
    int previous;
    int next;
};

#endif