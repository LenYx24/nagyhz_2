#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <memory>

#include "../include/menu.hpp"
#include "../include/statemanagement.hpp"

int main() {
  // Todo: remove try catch block, and only put it where its needed
  try {
    sf::RenderWindow window = sf::RenderWindow{sf::VideoMode{1280,800}, "lolmacrogame"};
    StateManager state_manager;
    state_manager.push_state(std::make_unique<Menu::MainState>(state_manager,window));
    while (state_manager.has_state()) {
      state_manager.handle_events(window);
      state_manager.update();
      state_manager.draw(window);
      state_manager.update_state();
    }
    window.close();
  } catch (const char *error) {
    std::cout << "Oops, there was an error: " << error << std::endl;
    exit(1);
  } catch (...) {
    std::cout << "error: unknown" << std::endl;
  }

  return 0;
}
