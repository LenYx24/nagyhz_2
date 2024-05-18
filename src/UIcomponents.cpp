#include "../include/UIcomponents.hpp"
using namespace UI;

Button::Button(sf::String str, std::function<void()> onclick, sf::Vector2f pos) {
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
void TextBox::draw(sf::RenderWindow& window){
  window.draw(shape);
  window.draw(text);
  window.draw(label);
}
bool Button::contains(int x, int y)const{
  return get_global_bounds().contains(static_cast<float>(x), static_cast<float>(y));
}
bool Grid::contains(int x, int y)const{
  return get_global_bounds().contains(static_cast<float>(x), static_cast<float>(y));
}
bool TextBox::contains(int x, int y)const{
  return get_global_bounds().contains(static_cast<float>(x), static_cast<float>(y));
}
bool NamedBox::contains(int x, int y)const{
  return get_global_bounds().contains(static_cast<float>(x), static_cast<float>(y));
}
TextBox::TextBox(std::string label_, Resources::Holder &holder, sf::Vector2f pos){
  shape.setFillColor(sf::Color::White);
  shape.setSize({200,50});
  shape.setOutlineColor({sf::Color::Black});
  shape.setOutlineThickness(3);
  shape.setPosition(pos);

  text.setString("");
  text.setCharacterSize(15);
  text.setFillColor(sf::Color{100,100,50});
  text.setFont(holder.get(Resources::Type::FONT));
  text.setPosition(pos + sf::Vector2f{5,5});

  label.setString(label_);
  label.setCharacterSize(15);
  label.setFillColor(sf::Color::Black);
  label.setFont(holder.get(Resources::Type::FONT));
  label.setPosition(pos - sf::Vector2f{150,0});
}
void TextBox::add_char(char c) {
  std::string str = text.getString();
  str.push_back(c);
  text.setString(str);
}
void TextBox::remove_char(){
  std::string str = text.getString();
  if(str.empty())return;
  str.pop_back();
  text.setString(str);
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
