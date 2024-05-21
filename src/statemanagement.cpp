#include <utility>

#include "../include/statemanagement.hpp"

void StateManager::change_state(std::unique_ptr<State> state) {
  pop = true;
  buffer_state = std::move(state);
}

void StateManager::push_state(std::unique_ptr<State> state) {
  buffer_state = std::move(state);
}

void StateManager::pop_state() {
  pop = true;
}
void StateManager::update_state(){
  if(pop){
    states.pop();
    pop = false;
  }
  if(buffer_state){
    states.push(std::move(buffer_state));
  }
  if (states.empty()) {
    exit();
  }
}
void StateManager::handle_events(sf::RenderWindow &window) {
  sf::Event e{};
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
    states.top()->draw(window);
  window.display();
}

// Todo: Don't forget to save to a file, before doing this
void StateManager::exit() {
  while (!states.empty())
    states.pop();
}

sf::Vector2f StateManager::get_size(sf::RenderWindow& window){
  sf::Vector2u window_size_unsigned = window.getSize();
  return sf::Vector2f{static_cast<float>(window_size_unsigned.x),static_cast<float>(window_size_unsigned.y)};
}

Settings::Settings(std::string champs_filepath, std::string items_filepath, GameMode mode){
  this->champs_filepath = std::move(champs_filepath);
  this->items_filepath = std::move(items_filepath);
  this->mode = mode;
}