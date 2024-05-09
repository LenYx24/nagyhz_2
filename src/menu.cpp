#include "../include/menu.hpp"

namespace Menu {
MenuButton::MenuButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}

void onclick_start(StateManager &s) {
  s.push_state(std::make_unique<ModeSelectionState>(s));
}

void onclick_back(StateManager &s) {
  s.pop_state();
}
MainState::MainState(StateManager &s) : MenuState(s) {
  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  buttons.push_back(MenuButton{h, "Start", onclick_start});
  buttons.push_back(MenuButton{h, "Settings"});
  buttons.push_back(MenuButton{h, "Exit", onclick_back});
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].getsize().y / 2.f;
  sf::Vector2f windowsize = _state_manager.getSize();
  sf::Vector2f pos{windowsize.x / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].setposition(pos);
    pos.y += buttons[i].getsize().y / 2.f + marginy;
  }
}

void MenuState::HandleEvents(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    _state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (UI::Button b : buttons) {
      if (b.getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b.onclick(_state_manager);
      }
    }
  }
}
void MenuState::Update() {
  // do update
}
void MenuState::Draw() {
  sf::Color background_color = sf::Color(220, 225, 222);
  sf::RenderWindow& window = _state_manager.getwindow();
  window.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].draw_to_window(window);
  }
}

void onclick_draft(StateManager &s) {
  Settings settings{"examples/champs.txt","examples/items.txt",GameMode::THEMSELVES};
  settings.m = GameMode::TWOPLAYER;
  s.push_state(std::make_unique<DraftState>(s, settings));
}
ModeSelectionState::ModeSelectionState(StateManager &s) : MenuState(s) {
  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  buttons.push_back(MenuButton{h, "Player vs Player", onclick_draft});
  buttons.push_back(MenuButton{h, "Player against yourself", onclick_draft});
  buttons.push_back(MenuButton{h, "back", onclick_back});
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].getsize().y / 2.f;
  sf::Vector2f windowsize = _state_manager.getSize();
  sf::Vector2f pos{windowsize.x / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].setposition(pos);
    pos.y += buttons[i].getsize().y / 2.f + marginy;
  }
}
} // namespace Menu
