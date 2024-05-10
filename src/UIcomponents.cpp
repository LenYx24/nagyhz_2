#include "../include/UIcomponents.hpp"
using namespace UI;

Button::Button(sf::String str, std::function<void(StateManager &s)> onclick, sf::Vector2f pos) {
  // default button settings
  text.setString(str);
  text.setFillColor({220, 225, 223});
  text.setCharacterSize(20);
  shape.setOutlineThickness(1);
  shape.setOutlineColor({31, 36, 33});
  shape.setFillColor({33, 104, 105});
  shape.setSize({500, 120});
  text.setOrigin({text.getLocalBounds().width / 2, text.getLocalBounds().height / 2});
  set_position(pos);
  this->onclick = onclick;
}

void Button::set_text(sf::String str) {
  text.setString(str);
}
void Button::set_position(sf::Vector2f pos) {
  shape.setOrigin(shape.getLocalBounds().width / 2.f, shape.getLocalBounds().height / 2.f);
  shape.setPosition(pos);
  update_text_position();
}
void Button::draw(sf::RenderWindow& window) {
  window.draw(shape);
  window.draw(text);
}
void Button::update_text_position() {
  text.setPosition(shape.getPosition().x - text.getLocalBounds().width / 2.f, shape.getPosition().y - text.getLocalBounds().height / 1.5f);
}
void Button::draw_to_window(sf::RenderWindow &w) {
  w.draw(shape);
  update_text_position();
  w.draw(text);
}
NamedBox::NamedBox(std::string label, sf::RectangleShape frame, Resources::Holder &h) {
  this->label.setString(label);
  this->label.setFont(h.get(Resources::Type::FONT));
  this->frame = frame;
}

void NamedBox::setcharsize(int size) {
  label.setCharacterSize(size);
}
void NamedBox::setlabelcolor(const sf::Color &c) {
  label.setFillColor(c);
}
void NamedBox::setlabel(std::string l){
  label.setString(l);
}

void NamedBox::set_position(sf::Vector2f pos) {
  frame.setPosition(pos);
}

sf::Vector2f NamedBox::get_size() {
  return frame.getSize();
}

void NamedBox::draw(sf::RenderWindow& w) {
  w.draw(frame);
  label.setPosition(frame.getPosition() + sf::Vector2f{5, 5});
  w.draw(label);
}

void Grid::setelementspos() {
  sf::Vector2f pos = _startpos;
  for (size_t i = 0; i < _elements.size(); i++) {
    _elements[i]->set_position(pos);
    pos.x += (_elements[i]->get_size().x + _margin.x) * _direction.x;
    pos.y += (_elements[i]->get_size().y + _margin.y) * _direction.y;
  }
}

void Grid::setelements(std::vector<GridElement *> elements) {
  _elements = elements;
}
Grid::Grid(sf::Vector2f startpos, sf::Vector2f margin, sf::Vector2f direction) : _startpos(startpos), _margin(margin), _direction(direction) {}
