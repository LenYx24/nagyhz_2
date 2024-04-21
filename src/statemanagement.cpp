#include "../include/statemanagement.hpp"

StateManager::StateManager() {}

void StateManager::ChangeState(std::unique_ptr<State> state) {
  if (!states.empty()) {
    states.pop_back();
  }
  states.push_back(std::move(state));
}
void StateManager::PushState(std::unique_ptr<State> state) {
  states.push_back(std::move(state));
  if (states[0]) {
    std::cout << "nincs out of scope" << std::endl;
  }
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
  if (!states.empty()) {
    states.back()->HandleEvents(*this, renderer);
  }
}
void StateManager::Update(Renderer &renderer) {
  if (!states.empty())
    states.back()->Update(*this, renderer);
}

// Todo: Don't forget to save to a file, before doing this
void StateManager::exit() {}
