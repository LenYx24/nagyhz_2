#ifndef ENGINE_HPP
#define ENGINE_HPP
#include <SFML/Graphics.hpp>

class State;
class Engine {
public:
  Engine(sf::VideoMode winsize = {800, 600});
  virtual ~Engine() {}

  void ChangeState(State &state);
  void PushState(State &state);
  void PopState();

  void HandleEvents();
  void Update();
  void Draw();

  inline sf::RenderWindow &getWindow() { return window; }

  inline bool isRunning() const { return is_running; }
  void exit();

private:
  std::vector<State *> states;
  bool is_running;
  sf::RenderWindow window;
  sf::VideoMode size;
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
