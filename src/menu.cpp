#include "../include/menu.hpp"

namespace Menu {
MenuButton::MenuButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}

void onclick_start(StateManager &s) {
  s.PushState(new ModeSelectionState);
}

void onclick_back(StateManager &s) {
  s.PopState();
}
MainState::MainState() {
  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  buttons.push_back(MenuButton{h, "Start", onclick_start});
  buttons.push_back(MenuButton{h, "Settings"});
  buttons.push_back(MenuButton{h, "Exit", onclick_back});
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].getSize().y / 2.f;
  sf::Vector2f pos{800 / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].setPosition(pos);
    pos.y += buttons[i].getSize().y / 2.f + marginy;
  }
}

void MenuState::HandleEvents(StateManager &s, Renderer &renderer) {
  for (sf::Event event{}; renderer.PollEvent(event);) {
    if (event.type == sf::Event::Closed) {
      s.exit();
    } else if (event.type == sf::Event::MouseButtonPressed) {
      std::cout << "[[INFO]] mouse clicked" << std::endl;
      for (Button b : buttons) {
        if (b.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
          b.onclick(s);
        }
      }
    }
  }
}
void MenuState::Update(StateManager &s, Renderer &renderer) {
  // update

  sf::RenderWindow &w = renderer.getWindow();
  sf::Color background_color = sf::Color(220, 225, 222);
  w.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].draw_to_window(w);
  }

  w.display();
}

ModeSelectionState::ModeSelectionState() {
  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  buttons.push_back(MenuButton{h, "Player vs Player"});
  buttons.push_back(MenuButton{h, "Player against yourself"});
  buttons.push_back(MenuButton{h, "back", onclick_back});
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].getSize().y / 2.f;
  sf::Vector2f pos{800 / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].setPosition(pos);
    pos.y += buttons[i].getSize().y / 2.f + marginy;
  }
}
} // namespace Menu
