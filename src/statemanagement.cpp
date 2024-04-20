#include "../include/statemanagement.hpp"

StateManager::StateManager() { has_states = false; }

void StateManager::ChangeState(State *state) {
    if (!states.empty()) {
        states.pop_back();
    }
    states.push_back(state);
}
void StateManager::PushState(State *state) {
    has_states = true;
    states.push_back(state);
}
void StateManager::PopState() {
    if (!states.empty())
        states.pop_back();
}
void StateManager::Update(Renderer &r) {
    if (!states.empty())
        states.back()->Update(*this, r);
}

void StateManager::exit() { has_states = false; }
