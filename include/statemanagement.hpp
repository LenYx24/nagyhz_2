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

  void ChangeState(std::unique_ptr<State> state);
  void PushState(std::unique_ptr<State> state);
  void PopState();

  void HandleEvents(Renderer &renderer);
  void Update(Renderer &renderer);

  inline bool hasState() const {
    return !states.empty();
  }
  void exit();

private:
  std::vector<std::unique_ptr<State>> states;
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
