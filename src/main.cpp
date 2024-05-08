#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <memory>

// my headers
#include "../include/menu.hpp"
#include "../include/statemanagement.hpp"

int main() {
  // Todo: remove try catch block, and only put it where its needed
  try {
    sf::RenderWindow window = sf::RenderWindow{sf::VideoMode{1280,800}, "lolmacrogame"};
    StateManager state_manager{window};
    state_manager.PushState(std::make_unique<Menu::MainState>(state_manager));
    while (state_manager.hasState()) {
      state_manager.HandleEvents();
      state_manager.Update();
      state_manager.Draw();
    }
    window.close();
  } catch (const char *err) {
    std::cout << "Oops, there was an error: " << err << std::endl;
    exit(1);
  } catch (...) {
    std::cout << "error: unknown" << std::endl;
  }

  return 0;
}
