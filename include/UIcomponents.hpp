#ifndef UICOMPONENTS_HPP
#define UICOMPONENTS_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>

#include "resources.hpp"
#include "statemanagement.hpp"

namespace UI {

class GridElement {
public:
  virtual ~GridElement() {}
  virtual void draw(sf::RenderWindow& window) = 0;
  virtual void set_position(sf::Vector2f pos) = 0;
  virtual bool contains(int x, int y)const=0;
  virtual sf::Vector2f get_size() = 0;
};

class Button : public GridElement {
public:
  Button() {}
  Button(
      sf::String text, std::function<void()> onclick = []() { std::cout << "onclick not implemented yet" << std::endl;},
      sf::Vector2f pos = {0, 0});
  void set_text(sf::String text);
  void set_position(sf::Vector2f pos) override;
  void update_text_position();
  void draw_to_window(sf::RenderWindow &window);
  bool contains(int x, int y)const;

  virtual void draw(sf::RenderWindow& window);
  inline sf::Vector2f get_size() {
    return shape.getSize();
  }
  inline sf::FloatRect get_global_bounds() const {return shape.getGlobalBounds();}
  std::function<void()> onclick;

protected:
  sf::RectangleShape shape;
  sf::Text text;
};
class TextBox : public GridElement {
public:
  TextBox(std::string label,Resources::Holder &holder, sf::Vector2f pos = {0, 0});
  virtual void draw(sf::RenderWindow& window);
  virtual void set_position(sf::Vector2f pos){shape.setPosition(pos);}
  virtual sf::Vector2f get_size(){return shape.getSize();}
  bool contains(int x, int y)const;
  inline sf::FloatRect get_global_bounds() const {
    return shape.getGlobalBounds();
  }
  void set_selected(bool s){is_selected = s;}
  bool get_is_selected()const{return is_selected;}
  void add_char(char c);
  void remove_char();
private:
  sf::Text text;
  sf::RectangleShape shape;
  sf::Text label;
  bool is_selected = false;
  int limit;
};
class Grid {
public:
  Grid(sf::Vector2f startpos, sf::Vector2f margin, sf::Vector2f direction = {1, 0});
  void setelements(std::vector<GridElement *> elements);
  void setelementspos();
  bool contains(int x, int y)const;
  void draw();
  inline sf::FloatRect get_global_bounds() const {
    sf::FloatRect f{};
    f.top = _startpos.y;
    f.left = _startpos.x;
    f.width = static_cast<float>(_elements.size())*(_margin.x+_direction.x);
    f.height = static_cast<float>(_elements.size())*(_margin.y+_direction.y);
    return f;
  }
private:
  std::vector<GridElement *> _elements;
  sf::Vector2f _startpos;
  sf::Vector2f _margin;
  sf::Vector2f _direction;
};
class NamedBox : public GridElement {
public:
  NamedBox(Resources::Holder &h) {label.setFont(h.get(Resources::Type::FONT));}
  NamedBox(std::string label, sf::RectangleShape frame, Resources::Holder &h);
  void setlabel(std::string l);
  void setframe(sf::RectangleShape);
  void set_position(sf::Vector2f pos);
  void setcharsize(int size);
  void setlabelcolor(const sf::Color &c);
  inline sf::FloatRect get_global_bounds() const {
    return frame.getGlobalBounds();
  }
  std::string getlabel() const {
    return label.getString();
  }
  bool contains(int x, int y)const;
  sf::Vector2f get_size();
  void draw(sf::RenderWindow& window);

protected:
  sf::RectangleShape frame;
  sf::Text label;
};

} // namespace UI

#endif
