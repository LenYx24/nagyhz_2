#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SFML/Graphics.hpp>

class State;
class Engine {
public:
  Engine();
  virtual ~Engine() {}

  void ChangeState(State &state);
  void PushState(State &state);
  void PopState();

  void HandleEvents();
  void Update();
  void Draw();

  inline sf::RenderWindow &getWindow() { return window; }

  inline bool isRunning() const { return is_running; }

private:
  std::vector<State *> states;
  bool is_running;
  sf::RenderWindow window;
};
class State {
public:
  virtual ~State() {}
  virtual void HandleEvents(Engine &e) = 0;
  virtual void Update(Engine &e) = 0;
  virtual void Draw(Engine &e) = 0;

  virtual void ChangeState(Engine &e) { e.ChangeState(*this); }
};

#endif
