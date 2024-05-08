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
  setposition(pos);
  onclick = onclick_;
}

void Button::settext(sf::String str) {
  text.setString(str);
}
void Button::setposition(sf::Vector2f pos) {
  shape.setOrigin(shape.getLocalBounds().width / 2.f, shape.getLocalBounds().height / 2.f);
  shape.setPosition(pos);
  updatetextpos();
}
void Button::draw(sf::RenderWindow& window) {
  window.draw(shape);
  window.draw(text);
}
void Button::updatetextpos() {
  text.setPosition(shape.getPosition().x - text.getLocalBounds().width / 2.f, shape.getPosition().y - text.getLocalBounds().height / 1.5f);
}
void Button::draw_to_window(sf::RenderWindow &w) {
  w.draw(shape);
  updatetextpos();
  w.draw(text);
}
NamedBox::NamedBox(std::string label, sf::RectangleShape frame, Resources::Holder &h) {
  _label.setString(label);
  _frame = frame;
  _label.setFont(h.get(Resources::Type::FONT));
}

void NamedBox::setcharsize(int size) {
  _label.setCharacterSize(size);
}
void NamedBox::setlabelcolor(const sf::Color &c) {
  _label.setFillColor(c);
}
  void NamedBox::setlabel(std::string l){
    _label.setString(l);
  }

void NamedBox::setposition(sf::Vector2f pos) {
  _frame.setPosition(pos);
}

sf::Vector2f NamedBox::getsize() {
  return _frame.getSize();
}

void NamedBox::draw(sf::RenderWindow& w) {
  w.draw(_frame);
  _label.setPosition(_frame.getPosition() + sf::Vector2f{5, 5});
  w.draw(_label);
}

void Grid::setelementspos() {
  sf::Vector2f pos = _startpos;
  for (size_t i = 0; i < _elements.size(); i++) {
    _elements[i]->setposition(pos);
    pos.x += (_elements[i]->getsize().x + _margin.x) * _direction.x;
    pos.y += (_elements[i]->getsize().y + _margin.y) * _direction.y;
  }
}

void Grid::setelements(std::vector<GridElement *> elements) {
  _elements = elements;
}
Grid::Grid(sf::Vector2f startpos, sf::Vector2f margin, sf::Vector2f direction) : _startpos(startpos), _margin(margin), _direction(direction) {}
