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
  virtual sf::Vector2f get_size() = 0;
};

class Button : public GridElement {
public:
  Button() {}
  Button(
      sf::String text, std::function<void(StateManager &s)> onclick = [](StateManager &state_manager) { std::cout << "onclick not implemented yet" << std::endl;},
      sf::Vector2f pos = {0, 0});
  void set_text(sf::String text);
  void set_position(sf::Vector2f pos);
  void update_text_position();
  void draw_to_window(sf::RenderWindow &window);

  virtual void draw(sf::RenderWindow& window);
  inline sf::Vector2f get_size() {
    return shape.getSize();
  }
  inline sf::FloatRect get_global_bounds() const {return shape.getGlobalBounds();}
  std::function<void(StateManager &s)> onclick;

protected:
  sf::RectangleShape shape;
  sf::Text text;
};
class TextBox : public GridElement {
  virtual void draw(sf::RenderWindow& window){}
  virtual void set_position(sf::Vector2f pos){}
  virtual sf::Vector2f get_size(){return shape.getSize();}

private:
  sf::Text textbox;
  sf::RectangleShape shape;
  bool isSelected;
  int width;
  int height;
  int limit;
};
class Grid {
public:
  Grid(sf::Vector2f startpos, sf::Vector2f margin, sf::Vector2f direction = {1, 0});
  void setelements(std::vector<GridElement *> elements);
  void setelementspos();
  void draw();

private:
  std::vector<GridElement *> _elements;
  sf::Vector2f _startpos;
  sf::Vector2f _margin;
  sf::Vector2f _direction;
};
class NamedTextBox : public GridElement {
public:
  virtual void draw(sf::RenderWindow& window);

private:
  TextBox textbox;
  sf::Text label;
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
  inline sf::FloatRect getglobalbounds() const {
    return frame.getGlobalBounds();
  }
  std::string getlabel() const {
    return label.getString();
  }

  sf::Vector2f get_size();
  void draw(sf::RenderWindow& window);

protected:
  sf::RectangleShape frame;
  sf::Text label;
};

} // namespace UI

#endif
