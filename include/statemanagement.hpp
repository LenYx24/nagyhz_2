#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stack>

enum class GameMode { THEMSELVES, TWOPLAYER };
class Settings {
public:
  std::string champs_filepath;
  std::string items_filepath;
  GameMode m;
};
class State;
class StateManager {
public:
  StateManager(sf::RenderWindow& window);

  void ChangeState(std::unique_ptr<State> state);
  void PushState(std::unique_ptr<State> state);
  void PopState();

  void HandleEvents();
  void Update();
  void Draw();

  inline sf::RenderWindow& getwindow()const { return window;}
  sf::Vector2f getSize()const;

  inline bool hasState() const {
    return !states.empty();
  }
  void exit();

private:
  std::stack<std::unique_ptr<State>> states;
  sf::RenderWindow &window;
};
class State {
public:
  virtual ~State() {}
  virtual void HandleEvents(sf::Event &e) = 0;
  virtual void Update() = 0;
  virtual void Draw() = 0;

protected:
  State(StateManager &s) : _state_manager(s) {}
  StateManager &_state_manager;
};

#endif
