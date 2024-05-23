#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <utility>

#include "UIcomponents.hpp"
#include "draft.hpp"
#include "resources.hpp"


namespace Menu {
/**
 * @brief the general menustate class, used as a base for simple menus
 */
class MenuState : public State {
public:
  explicit MenuState(StateManager &state_manager,
                     Settings setting) :
        State(state_manager),
        setting(std::move(setting)){}
  ~MenuState() override;
  void handle_events(sf::Event &event) override;
  void update() override;
  void draw(sf::RenderWindow& window) override;

protected:
  std::vector<UI::Button *> buttons;
  Resources::Holder resources_holder;
  Settings setting;
};
/*
 * @brief a specialized state which implements the main state of this application
 */
class MainState : public MenuState {
public:
  MainState(StateManager &s, sf::RenderWindow& window, Settings setting);
  ~MainState() override;
  void handle_events(sf::Event &event) override;
  void draw(sf::RenderWindow& window) override;
private:
  std::map<std::string,UI::TextBox *> text_boxes;
};
/*
 * @brief a small state that handles selecting the mode of the game
 */
class ModeSelectionState : public MenuState {
public:
  ModeSelectionState(StateManager &state_manager, sf::RenderWindow& window, Settings setting);
};
/*
 * @brief a specialized button that gives the default style for a menu button
 */
class MenuButton : public UI::Button {
public:
  MenuButton(
      Resources::Holder &h, const sf::String& str, std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; });
};

} // namespace Menu
#endif
// !MENU
