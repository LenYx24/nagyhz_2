#include "../include/menu.hpp"

namespace Menu {

MenuButton::MenuButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}
void onclick_back(StateManager &state_manager){
  state_manager.pop_state();
}
MainState::MainState(StateManager &state_manager, sf::RenderWindow &window) : MenuState(state_manager) {
  resources_holder.load(Resources::Type::FONT, "./fonts/Roboto.ttf");

  std::function<void(StateManager &state_manager)> onclick_start = [&window](StateManager &state_manager){state_manager.push_state(std::make_unique<ModeSelectionState>(state_manager,window));};
  buttons.push_back(MenuButton{resources_holder, "Start", onclick_start});
  buttons.push_back(MenuButton{resources_holder, "Settings"});
  buttons.push_back(MenuButton{resources_holder, "Exit", onclick_back});
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].get_size().y / 2.f;
  sf::Vector2f windowsize = state_manager.get_size(window);
  sf::Vector2f pos{windowsize.x / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].set_position(pos);
    pos.y += buttons[i].get_size().y / 2.f + marginy;
  }
}

void MenuState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (UI::Button b : buttons) {
      if (b.get_global_bounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b.onclick(state_manager);
      }
    }
  }
}
void MenuState::update() {}
void MenuState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].draw_to_window(window);
  }
}
ModeSelectionState::ModeSelectionState(StateManager &s, sf::RenderWindow& window) : MenuState(s) {
  resources_holder.load(Resources::Type::FONT, "./fonts/Roboto.ttf");

  std::function<void(StateManager& s)> onclick_draft = [&s, &window](StateManager& state_manager){
    Settings settings{"examples/champs.txt","examples/items.txt",GameMode::THEMSELVES};
    s.push_state(std::make_unique<DraftState>(s, settings, window));
  };

  buttons.push_back(MenuButton{resources_holder, "Player vs Player", onclick_draft});
  buttons.push_back(MenuButton{resources_holder, "Player against yourself", onclick_draft});
  buttons.push_back(MenuButton{resources_holder, "back", onclick_back});
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].get_size().y / 2.f;
  sf::Vector2f windowsize = state_manager.get_size(window);
  sf::Vector2f pos{windowsize.x / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].set_position(pos);
    pos.y += buttons[i].get_size().y / 2.f + marginy;
  }
}
} // namespace Menu
