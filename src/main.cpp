#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>

// my headers
#include "../include/engine.hpp"
#include "../include/menu.hpp"

int main() {
  Engine engine;
  engine.PushState(*new Menu::MenuState);
  while (engine.isRunning()) {
    engine.HandleEvents();
    engine.Update();
    engine.Draw();
  }
  return 0;
}
