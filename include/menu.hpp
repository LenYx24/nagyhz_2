#ifndef MENU_HPP
#define MENU_HPP

#include "UIcomponents.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace Menu {

class MenuButton : public Button {
public:
  MenuButton(
      Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick = [](StateManager &s) { std::cout << "not impl" << std::endl; });
};

class MenuState : public State {
public:
  MenuState() {}

  virtual void HandleEvents(StateManager &s, Renderer &renderer);
  virtual void Update(StateManager &s, Renderer &r);

protected:
  std::vector<Button> buttons;
};
class ModeSelectionState : public MenuState {
public:
  ModeSelectionState();
};
class MainState : public MenuState {
public:
  MainState();
};
// Todo: create settings state, which consists namedtextbox components, where you can set the file names where you want to read and write to
//    also it could have a save button, to save the changes

} // namespace Menu
#endif // !MENU
