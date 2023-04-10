/*
AEON_FSM.cpp

*/

#include "AEON_FSM.h"

Transition *ALWAYS_FALSE_TRANSITION = new Transition(
    -1, []{ return false; },
    NULL, -1);

bool dummy_guard(void)
{
    return true;
}

void dummy_callback(void) 
{
    return;
}   

callback cb(callback c)
{
    return *c == NULL ? dummy_callback : c;
}

guard gd(guard g)
{
    return *g == NULL ? dummy_guard : g;
}

Transition::Transition(
    EventId eventId,
    guard fnGuard,
    callback fnOnTransition,
    StateId nextState)
{
    this->eventId = eventId;
    this->fnOnTransition = cb(fnOnTransition);
    this->fnGuard = gd(fnGuard);
    this->nextStateId = nextState;
}

State::State(
    FSM *parent,
    StateId stateId,
    callback fnOnEnterState,
    callback fnOnExitState,
    callback fnOnStayInState)
{
    this->parent = parent;
    this->stateId = stateId;
    this->fnOnEnterState =  cb(fnOnEnterState);
    this->fnOnExitState =  cb(fnOnExitState);
    this->fnOnStayInState = cb(fnOnStayInState);

}

State *State::addTransition(
    EventId eventId,
    guard fnGuard,
    callback fnOnTransition,
    StateId nextState)
{

    this->transitions[eventId] = new Transition(eventId, fnGuard, fnOnTransition, nextState);

    return this;
}

FSM *State::end()
{
    return this->parent;
}

State *FSM::addState(
    StateId stateId,
    callback fnOnEnterState,
    callback fnOnExitState,
    callback fnOnStayInState)
{
    this->states[stateId] = new State(this, stateId, fnOnEnterState, fnOnExitState, fnOnStayInState);

    return this->states[stateId];
}

void FSM::setCurrentStateId(StateId initialState)
{
    this->currentStateId = initialState;
}

StateId FSM::getCurrentStateId(void)
{
    return this->currentStateId;
}

bool FSM::dispatch(EventId e)
{
    State *currentState = this->states[this->currentStateId];

    Transition *currentTransition = currentState->transitions[e];
    currentTransition = currentTransition == NULL ? ALWAYS_FALSE_TRANSITION : currentTransition;

    if (!currentTransition->fnGuard())
    {
        currentState->fnOnStayInState();
        return false;
    }

    currentState->fnOnExitState();

    currentTransition->fnOnTransition();

    this->setCurrentStateId(currentTransition->nextStateId);

    currentState = this->states[this->currentStateId];

    currentState->fnOnEnterState();

    return true;
}