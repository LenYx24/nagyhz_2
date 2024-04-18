#ifndef MENU_HPP
#define MENU_HPP

#include "engine.hpp"
#include <iostream>

namespace Menu {

class Button {
public:
  sf::RectangleShape shape{sf::Vector2f(20, 20)};
  sf::Vector2i pos{20, 20};
};

class MenuState : public State {
public:
  MenuState();
  virtual ~MenuState();

  virtual void HandleEvents(Engine &e);
  virtual void Update(Engine &e);
  virtual void Draw(Engine &e);

private:
  Button *buttons;
};

} // namespace Menu
#endif // !MENU
