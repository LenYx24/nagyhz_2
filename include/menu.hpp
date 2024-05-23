#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

#include "UIcomponents.hpp"
#include "draft.hpp"
#include "resources.hpp"


namespace Menu {

class MenuState : public State {
public:
  explicit MenuState(StateManager &state_manager) :
        State(state_manager),
        setting("examples/champions.txt",
                "examples/items.txt",
                "examples/game",
                GameMode::TWO_PLAYER){}
  ~MenuState() override;
  void handle_events(sf::Event &event) override;
  void update() override;
  void draw(sf::RenderWindow& window) override;

protected:
  std::vector<UI::Button *> buttons;
  Resources::Holder resources_holder;
  Settings setting;
};
class MainState : public MenuState {
public:
  MainState(StateManager &s, sf::RenderWindow& window);
  ~MainState() override;
  void handle_events(sf::Event &event) override;
  void draw(sf::RenderWindow& window) override;
private:
  std::map<std::string,UI::TextBox *> text_boxes;
};
class ModeSelectionState : public MenuState {
public:
  ModeSelectionState(StateManager &s, sf::RenderWindow& window);
};

class MenuButton : public UI::Button {
public:
  MenuButton(
      Resources::Holder &h, const sf::String& str, std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; });
};

} // namespace Menu
#endif
// !MENU
