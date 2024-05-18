#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stack>

class State;
class StateManager {
public:
  StateManager():pop(false){buffer_state = std::move(std::unique_ptr<State>(nullptr));}
  void change_state(std::unique_ptr<State> state);
  void push_state(std::unique_ptr<State> state);
  void pop_state();
  void update_state();

  void handle_events(sf::RenderWindow &window);
  void update();
  void draw(sf::RenderWindow &window);

  sf::Vector2f get_size(sf::RenderWindow& window)const;
  inline bool has_state() const {return !states.empty() || buffer_state;}

  void exit();

private:
  std::stack<std::unique_ptr<State>> states;
  std::unique_ptr<State> buffer_state;
  bool pop;
};
class State {
public:
  virtual ~State() {}
  virtual void handle_events(sf::Event &event) = 0;
  virtual void update() = 0;
  virtual void draw(sf::RenderWindow& window) = 0;

protected:
  State(StateManager &state_manager) : state_manager(state_manager) {}
  StateManager &state_manager;
};
enum class GameMode { TWOPLAYER };
struct Settings {
public:
  Settings(std::string champs_filepath, std::string items_filepath, GameMode mode);
  std::string champs_filepath;
  std::string items_filepath;
  GameMode mode;
};
#endif
