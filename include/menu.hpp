#ifndef MENU
#define MENU

#include "SFML/Graphics.hpp"
#include <SFML/System/Vector2.hpp>

namespace Menu {
class Button {
public:
  sf::RectangleShape shape{sf::Vector2f(20, 20)};
  sf::Vector2i pos{20, 20};
};

} // namespace Menu
#endif // !MENU
