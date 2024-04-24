#include "../include/statemanagement.hpp"

StateManager::StateManager() {}

void StateManager::ChangeState(std::unique_ptr<State> state) {
  if (!states.empty()) {
    states.pop();
  }
  states.push(std::move(state));
}
void StateManager::PushState(std::unique_ptr<State> state) {
  states.push(std::move(state));
}
void StateManager::PopState() {
  if (!states.empty()) {
    states.pop();
  }
  if (states.empty()) {
    exit();
  }
}

void StateManager::HandleEvents(sf::RenderWindow &w) {
  sf::Event e;
  while (w.pollEvent(e))
    states.top()->HandleEvents(e);
}
void StateManager::Update() {
  states.top()->Update();
}
void StateManager::Draw(sf::RenderWindow &window) {
  states.top()->Draw(window);
  window.display();
}

// Todo: Don't forget to save to a file, before doing this
void StateManager::exit() {
  while (!states.empty())
    states.pop();
}
