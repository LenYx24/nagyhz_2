#include "../include/menu.hpp"

namespace Menu {

MenuState::MenuState() {
  buttons = new Button[3];
  buttons[0].setpos(sf::Vector2f{0, 0});
  buttons[1].setpos(sf::Vector2f{250, 100});
  buttons[2].setpos(sf::Vector2f{800, 600});
  buttons[0].settext("Start");
  buttons[1].settext("Settings");
  buttons[2].settext("Exit");
}
MenuState::~MenuState() { delete[] buttons; }

void MenuState::HandleEvents(Engine &e) {
  sf::RenderWindow &w = e.getWindow();
  for (auto event = sf::Event{}; w.pollEvent(event);) {
    if (event.type == sf::Event::Closed) {
      e.exit();
    } else if (event.type == sf::Event::MouseButtonPressed) {
      std::cout << "[[INFO]] mouse clicked" << std::endl;
    }
  }
}
void MenuState::Draw(Engine &e) {
  sf::RenderWindow &w = e.getWindow();
  sf::Color c = sf::Color(150, 124, 200, 100);
  w.clear(c);
  for (int i = 0; i < 3; i++) {
    w.draw(buttons[i]);
    w.draw(buttons[i].gettext());
  }
  w.display();
}
void MenuState::Update(Engine &e) {
  sf::RenderWindow &w = e.getWindow();
  w.draw(sf::RectangleShape());
  // update
}
} // namespace Menu
