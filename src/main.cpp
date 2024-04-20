#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>
#include <memory>

// my headers
#include "../include/menu.hpp"
#include "../include/renderer.hpp"
#include "../include/statemanagement.hpp"

int main() {
  // Todo: remove try catch
  try {
    StateManager state_mngr;
    Renderer renderer{sf::VideoMode{800, 600}, "lolmacrogame"};
    std::unique_ptr<Menu::MainState> state = std::make_unique<Menu::MainState>();
    state_mngr.PushState(std::move(state));
    while (state_mngr.hasState()) {
      state_mngr.HandleEvents(renderer);
      state_mngr.Update(renderer);
    }
  } catch (const char *err) {
    std::cout << "Oops, there was an error: " << err << std::endl;
    exit(1);
  } catch (...) {
    std::cout << "error: unknown" << std::endl;
  }

  return 0;
}
