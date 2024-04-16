#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

// my headers
#include "../include/menu.hpp"

int main() {
  auto window = sf::RenderWindow{{800, 600}, "lol macro game"};
  window.setFramerateLimit(144);
  Menu::Button b;

  while (window.isOpen()) {
    for (auto event = sf::Event{}; window.pollEvent(event);) {
      if (event.type == sf::Event::Closed) {
        window.close();
      } else if (event.type == sf::Event::MouseLeft) {
      }
    }
    window.clear();

    window.draw(b.shape);
    window.display();
  }
}
