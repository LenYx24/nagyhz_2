#include "../include/statemanagement.hpp"

StateManager::StateManager() {
  has_states = false;
}

void StateManager::ChangeState(State *state) {
  if (!states.empty()) {
    states.pop_back();
  }
  states.push_back(std::unique_ptr<State>(state));
}
void StateManager::PushState(State *state) {
  has_states = true;
  states.push_back(std::unique_ptr<State>(state));
}
void StateManager::PopState() {
  if (!states.empty()) {
    states.pop_back();
  }
  if (states.empty()) {
    exit();
  }
}

void StateManager::HandleEvents(Renderer &renderer) {
  if (!states.empty())
    states.back()->HandleEvents(*this, renderer);
}
void StateManager::Update(Renderer &renderer) {
  if (!states.empty())
    states.back()->Update(*this, renderer);
}

void StateManager::exit() {
  has_states = false;
}
