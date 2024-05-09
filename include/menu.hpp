#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

#include "UIcomponents.hpp"
#include "draft.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"


namespace Menu {

class MenuState : public State {
public:
  MenuState(StateManager &s) : State(s) {}

  virtual void HandleEvents(sf::Event &e);
  virtual void Update();
  virtual void Draw();

protected:
  std::vector<UI::Button> buttons;
  Resources::Holder h;
};
class MainState : public MenuState {
public:
  MainState(StateManager &s, sf::RenderWindow& window);
};
class ModeSelectionState : public MenuState {
public:
  ModeSelectionState(StateManager &s);
};

class MenuButton : public UI::Button {
public:
  MenuButton(
      Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick = [](StateManager &s) { std::cout << "not impl" << std::endl; });
};

} // namespace Menu
#endif // !MENU
