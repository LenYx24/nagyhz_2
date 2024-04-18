#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

// my headers
#include "../include/engine.hpp"
#include "../include/menu.hpp"

int main() {
  try {
    Engine engine;
    engine.PushState(*(new Menu::MenuState));
    while (engine.isRunning()) {
      engine.HandleEvents();
      engine.Update();
      engine.Draw();
    }
  } catch (const char *err) {
    std::cout << "Oops, there was an error: " << err << std::endl;
    exit(1);
  }

  return 0;
}
