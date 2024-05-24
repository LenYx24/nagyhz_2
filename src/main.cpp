#include <SFML/Graphics.hpp>

#include "../include/menu.hpp"

int main() {
  // seed the random generator
  srand(static_cast<unsigned>(time(nullptr)));
  sf::RenderWindow window = sf::RenderWindow{sf::VideoMode{1280,800}, "lolmacrogame"};
  StateManager state_manager;
  Settings settings{"examples/champions.txt",
                    "examples/items.txt",
                    "examples/game",
                    GameMode::TWO_PLAYER};
  state_manager.push_state(std::make_unique<Menu::MainState>(state_manager,window, settings));
  while (state_manager.has_state()) {
    state_manager.handle_events(window);
    state_manager.update();
    state_manager.draw(window);
    state_manager.update_state();
  }
  window.close();
  return 0;
}
