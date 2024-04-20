#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "renderer.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

class State;
class StateManager {
public:
  StateManager();

  void ChangeState(State *state);
  void PushState(State *state);
  void PopState();

  void HandleEvents(Renderer &renderer);
  void Update(Renderer &renderer);

  inline bool hasState() const {
    return has_states;
  }
  void exit();

private:
  std::vector<std::unique_ptr<State>> states;
  bool has_states;
};
class State {
public:
  virtual ~State() {}
  virtual void HandleEvents(StateManager &s, Renderer &renderer) = 0;
  virtual void Update(StateManager &s, Renderer &r) = 0;
};

#endif
