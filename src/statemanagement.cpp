#include "../include/statemanagement.hpp"

StateManager::StateManager(sf::RenderWindow& window):window(window) {}

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

void StateManager::HandleEvents() {
  sf::Event e;
  while (window.pollEvent(e))
    states.top()->HandleEvents(e);
}
void StateManager::Update() {
  if(!states.empty()) // need to check this, because I didn't implemented the feature, that only changes state at the end of the main loop
    states.top()->Update();
}
void StateManager::Draw() {
  if(!states.empty())
    states.top()->Draw();
  window.display();
}

// Todo: Don't forget to save to a file, before doing this
void StateManager::exit() {
  while (!states.empty())
    states.pop();
}
sf::Vector2f StateManager::getSize()const{
  sf::Vector2u wsizeu = window.getSize();
  return sf::Vector2f{static_cast<float>(wsizeu.x),static_cast<float>(wsizeu.y)};
}