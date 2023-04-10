/*
AEON_FSM.h
*/

#include <Arduino.h>
#include <stdlib.h>
#include <map>
#include <string.h>

typedef int EventId;

typedef int StateId;

typedef void (*callback)(void);

typedef bool (*guard)(void);

class Transition
{
    friend class FSM;

private:
    EventId eventId;
    callback fnOnTransition;
    guard fnGuard;
    StateId nextStateId;

public:
    Transition(
        EventId eventId,
        guard fnGuard,
        callback fnOnTransition,
        StateId nextStateId);
};

class FSM;

class State
{
    friend class FSM;

private:
    StateId stateId;
    FSM *parent;
    callback fnOnEnterState;
    callback fnOnExitState;
    callback fnOnStayInState;

    std::map<EventId, Transition *> transitions{};

public:
    State(
        FSM *parent,
        StateId stateId,
        callback fnOnEnterState,
        callback fnOnExitState,
        callback fnOnStayInState);

    State *addTransition(
        EventId eventId,
        guard fnGuard,
        callback fnOnTransition,
        StateId nextStateId);

    FSM *end();
};

class FSM
{

private:
    std::map<StateId, State *> states;
    StateId currentStateId;

public:
    State *addState(
        StateId stateId,
        callback fnOnEnterState,
        callback fnOnExitState,
        callback fnOnStayInState);

    void setCurrentStateId(StateId initialState);
    StateId getCurrentStateId(void);

    // zustand wurde gewechselt
    bool dispatch(EventId e);
};