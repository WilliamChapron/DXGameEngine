#pragma once
#include <iostream>
#include <vector>

class State;

class StateMachine
{
public:
    void AddState(State* state);

    void ChangeState(int index);

    void UpdateCurrentState();

private:
    std::vector<State*> states; // Liste des états
    State* currentState = nullptr; // État actuel
};

