#include "../include/menu.hpp"

namespace Menu {

MenuState::MenuState() { buttons = new Button[3]; }
MenuState::~MenuState() { delete[] buttons; }

void MenuState::HandleEvents(Engine &e) {
  sf::RenderWindow &w = e.getWindow();
  for (auto event = sf::Event{}; w.pollEvent(event);) {
    if (event.type == sf::Event::Closed) {
      w.close();
    } else if (event.type == sf::Event::MouseButtonPressed) {
      std::cout << "[[INFO]] mouse clicked" << std::endl;
    }
  }
}
void MenuState::Draw(Engine &e) {

  sf::RenderWindow &w = e.getWindow();
  w.draw(sf::RectangleShape());
  // draw
}
void MenuState::Update(Engine &e) {
  sf::RenderWindow &w = e.getWindow();
  w.draw(sf::RectangleShape());
  // update
}
} // namespace Menu
