#ifndef UICOMPONENTS_HPP
#define UICOMPONENTS_HPP
#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>
// Todo: add UI namespace
namespace UI {

class GridElement {
public:
  virtual void draw(sf::RenderWindow &w) = 0;
  virtual sf::Vector2f getsize() = 0;
  virtual void setposition(sf::Vector2f pos) = 0;
};
class Button : public GridElement {
public:
  Button() {}
  Button(
      sf::String str, std::function<void(StateManager &s)> onclick_ = [](StateManager &s) { std::cout << "onclick not implemented yet" << std::endl; },
      sf::Vector2f pos = {0, 0});
  void settext(sf::String str);
  void setposition(sf::Vector2f pos);
  void updatetextpos();
  void draw_to_window(sf::RenderWindow &w);

  virtual void draw(sf::RenderWindow &w);
  inline sf::Vector2f getsize() {
    return shape.getSize();
  }
  inline sf::FloatRect getglobalbounds() const {
    return shape.getGlobalBounds();
  }
  std::function<void(StateManager &s)> onclick;

protected:
  sf::RectangleShape shape;
  sf::Text text;
};
class TextBox : public GridElement {
  virtual void draw(sf::RenderWindow &w);
  virtual void setposition(sf::Vector2f pos);
  virtual sf::Vector2f getsize();

private:
  sf::Text textbox;
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
  void draw(sf::RenderWindow &window);

private:
  std::vector<GridElement *> _elements;
  sf::Vector2f _startpos;
  sf::Vector2f _margin;
  sf::Vector2f _direction;
};
class NamedTextBox : public GridElement {
public:
  virtual void draw(sf::RenderWindow &w);

private:
  TextBox tb;
  sf::Text label;
};
class NamedBox : public GridElement {
public:
  NamedBox();
  NamedBox(sf::RectangleShape);
  NamedBox(std::string label, sf::RectangleShape frame);
  void setlabel(std::string l);
  void setframe(sf::RectangleShape);
  void setposition(sf::Vector2f pos);

  sf::Vector2f getsize();
  void draw(sf::RenderWindow &w);

protected:
  sf::RectangleShape _frame;
  sf::Text _label;
};

} // namespace UI

#endif
