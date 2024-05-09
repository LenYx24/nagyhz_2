#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stack>

class State;
class StateManager {
public:
  void change_state(std::unique_ptr<State> state);
  void push_state(std::unique_ptr<State> state);
  void pop_state();

  void handle_events(sf::RenderWindow &window);
  void update();
  void draw(sf::RenderWindow &window);

  sf::Vector2f get_size(sf::RenderWindow &window)const;
  inline bool has_state() const {return !states.empty();}

  void exit();

private:
  std::stack<std::unique_ptr<State>> states;
};
class State {
public:
  virtual ~State() {}
  virtual void handle_events(sf::Event &event) = 0;
  virtual void update() = 0;
  virtual void draw() = 0;

protected:
  State(StateManager &state_manager) : state_manager(state_manager) {}
  StateManager &state_manager;
};
enum class GameMode { THEMSELVES, TWOPLAYER };
class Settings {
public:
  Settings(std::string champs_filepath, std::string items_filepath, GameMode mode);

private:
  std::string champs_filepath;
  std::string items_filepath;
  GameMode mode;
};
#endif
