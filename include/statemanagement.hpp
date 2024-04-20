#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "renderer.hpp"
#include "resources.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

enum class GameMode { THEMSELVES, TWOPLAYER };
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
  std::vector<State *> states;
  bool has_states;
};
class State {
public:
  virtual ~State() {}
  virtual void HandleEvents(StateManager &s, Renderer &renderer) = 0;
  virtual void Update(StateManager &s, Renderer &r) = 0;

protected:
  Resources::Holder h;
};

#endif
