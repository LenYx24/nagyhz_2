#include <utility>

#include "../include/UIcomponents.hpp"
using namespace UI;

Button::Button(const sf::String& str, std::function<void()> onclick, sf::Vector2f pos) {
  // default button settings
  text.setString(str);
  text.setFillColor({220, 225, 223});
  text.setCharacterSize(20);
  shape.setOutlineThickness(1);
  shape.setOutlineColor({31, 36, 33});
  shape.setFillColor({33, 104, 105});
  shape.setSize({500, 120});
  text.setOrigin({text.getLocalBounds().width / 2, text.getLocalBounds().height / 2});
  shape.setPosition(pos);
  text.setPosition(pos);
  this->onclick = std::move(onclick);
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
TextBox::TextBox(const std::string& label_, Resources::Holder &holder, sf::Vector2f pos, const std::string& text_default){
  shape.setFillColor(sf::Color::White);
  shape.setSize({200,50});
  shape.setOutlineColor({sf::Color::Black});
  shape.setOutlineThickness(3);
  shape.setPosition(pos);

  text.setString(text_default);
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
NamedBox::NamedBox(const std::string& label, sf::RectangleShape frame, Resources::Holder &h) {
  this->label.setString(label);
  this->label.setFont(h.get(Resources::Type::FONT));
  this->frame = std::move(frame);
}

void NamedBox::set_char_size(unsigned size) {
  label.setCharacterSize(size);
}
void NamedBox::set_label_color(const sf::Color &c) {
  label.setFillColor(c);
}
void NamedBox::set_label(const std::string& label_test){
  label.setString(label_test);
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

void Grid::set_elements_pos() {
  sf::Vector2f pos = start_pos;
  for (auto & element : elements) {
    element->set_position(pos);
    pos.x += (element->get_size().x + margin.x) * direction.x;
    pos.y += (element->get_size().y + margin.y) * direction.y;
  }
}
sf::FloatRect Grid::get_global_bounds() const {
  sf::FloatRect f{};
  f.top = start_pos.y;
  f.left = start_pos.x;
  f.width = static_cast<float>(elements.size())*(margin.x+ direction.x);
  f.height = static_cast<float>(elements.size())*(margin.y+ direction.y);
  return f;
}
void Grid::set_elements(std::vector<GridElement *> elements_) {
  elements = std::move(elements_);
}
Grid::Grid(sf::Vector2f start_pos, sf::Vector2f margin_, sf::Vector2f direction_) : start_pos(start_pos), margin(margin_), direction(direction_) {}
