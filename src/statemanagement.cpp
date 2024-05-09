#include "../include/statemanagement.hpp"

void StateManager::change_state(std::unique_ptr<State> state) {
  if (!states.empty()) {
    states.pop();
  }
  states.push(std::move(state));
}
void StateManager::push_state(std::unique_ptr<State> state) {
  states.push(std::move(state));
}
void StateManager::pop_state() {
  if (!states.empty()) {
    states.pop();
  }
  if (states.empty()) {
    exit();
  }
}

void StateManager::handle_events(sf::RenderWindow &window) {
  sf::Event e;
  while (window.pollEvent(e)){
    if(!states.empty())
      states.top()->handle_events(e);
  }
}
void StateManager::update() {
  // Todo: implement the feature that does state changes after a game loop cycle
  if(!states.empty()) 
    states.top()->update();
}
void StateManager::draw(sf::RenderWindow &window) {
  if(!states.empty())
    states.top()->draw();
  window.display();
}

// Todo: Don't forget to save to a file, before doing this
void StateManager::exit() {
  while (!states.empty())
    states.pop();
}
sf::Vector2f StateManager::get_size(sf::RenderWindow &window)const{
  sf::Vector2u wsizeu = window.getSize();
  return sf::Vector2f{static_cast<float>(wsizeu.x),static_cast<float>(wsizeu.y)};
}
Settings::Settings(std::string champs_filepath, std::string items_filepath, GameMode mode){
  this->champs_filepath = champs_filepath;
  this->items_filepath = items_filepath;
  this->mode = mode;
}