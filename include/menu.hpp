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
  MenuState(StateManager &state_manager) : State(state_manager){}

  virtual void handle_events(sf::Event &event);
  virtual void update();
  virtual void draw(sf::RenderWindow& window);

protected:
  std::vector<UI::Button *> buttons;
  Resources::Holder resources_holder;
};
class MainState : public MenuState {
public:
  MainState(StateManager &s, sf::RenderWindow& window);
};
class ModeSelectionState : public MenuState {
public:
  ModeSelectionState(StateManager &s, sf::RenderWindow& window);
};

class MenuButton : public UI::Button {
public:
  MenuButton(
      Resources::Holder &h, sf::String str, std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; });
};

} // namespace Menu
#endif // !MENU
