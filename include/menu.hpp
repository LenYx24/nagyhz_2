#ifndef MENU_HPP
#define MENU_HPP

#include "engine.hpp"
#include <iostream>

namespace Menu {

class Button : public sf::RectangleShape {
public:
  Button(sf::Vector2f pos = {20, 20}, sf::String str = "") {
    if (!font.loadFromFile("fonts/arial.ttf")) {
      throw "Error: Could not resolve \"arial\" font";
    }
    text.setFont(font);
    text.setString(str);
    text.setCharacterSize(20);
    setOutlineThickness(1);
    setOutlineColor({255, 231, 11});
    setFillColor({100, 230, 111});
    this->setPosition(pos);
    setSize({100, 50});
    settextpos();
  }
  void settext(sf::String str = "") { text.setString(str); }
  void setpos(sf::Vector2f pos) {
    this->setPosition(pos);
    settextpos();
  }
  void settextpos() {
    // todo: set it to center
    text.setOrigin(
        getPosition().x + getSize().x / 2 - getLocalBounds().width / 2,
        getPosition().y + getSize().y / 2 - getLocalBounds().height / 2);
    text.setPosition(getPosition() + (getSize() / 2.f));
  }
  sf::Text gettext() { return text; }

private:
  sf::Text text;
  sf::Font font;
};

class MenuState : public State {
public:
  MenuState();
  virtual ~MenuState();

  virtual void HandleEvents(Engine &e);
  virtual void Update(Engine &e);
  virtual void Draw(Engine &e);

private:
  Button *buttons;
};

} // namespace Menu
#endif // !MENU
