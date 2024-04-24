#include "../include/UIcomponents.hpp"
using namespace UI;

Button::Button(sf::String str, std::function<void(StateManager &s)> onclick_, sf::Vector2f pos) {
  // default button settings
  text.setString(str);
  text.setFillColor({220, 225, 223});
  text.setCharacterSize(20);
  shape.setOutlineThickness(1);
  shape.setOutlineColor({31, 36, 33});
  shape.setFillColor({33, 104, 105});
  shape.setSize({500, 120});
  text.setOrigin({text.getLocalBounds().width / 2, text.getLocalBounds().height / 2});
  setpos(pos);
  onclick = onclick_;
}

void Button::settext(sf::String str) {
  text.setString(str);
}
void Button::setpos(sf::Vector2f pos) {
  shape.setOrigin(shape.getLocalBounds().width / 2.f, shape.getLocalBounds().height / 2.f);
  shape.setPosition(pos);
  updatetextpos();
}
void Button::updatetextpos() {
  text.setPosition(shape.getPosition().x - text.getLocalBounds().width / 2.f, shape.getPosition().y - text.getLocalBounds().height / 1.5f);
}
void Button::draw_to_window(sf::RenderWindow &w) {
  w.draw(shape);
  updatetextpos();
  w.draw(text);
}
NamedBox::NamedBox() {}
NamedBox::NamedBox(std::string label, sf::RectangleShape frame) {
  _label.setString(label);
  _frame = frame;
}

NamedBox::NamedBox(sf::RectangleShape frame) {
  _frame = frame;
}

void NamedBox::setposition(sf::Vector2f pos) {
  _frame.setPosition(pos);
}

sf::Vector2f NamedBox::getsize() {
  return _frame.getSize();
}

void NamedBox::draw(sf::RenderWindow &w) {
  w.draw(_frame);
  w.draw(_label);
}
