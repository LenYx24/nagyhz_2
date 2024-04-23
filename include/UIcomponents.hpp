#ifndef UICOMPONENTS_HPP
#define UICOMPONENTS_HPP
#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>
// Todo: add UI namespace
class Button : public sf::RectangleShape {
public:
  Button() {}
  Button(
      sf::String str, std::function<void(StateManager &s)> onclick_ = [](StateManager &s) { std::cout << "onclick not implemented yet" << std::endl; },
      sf::Vector2f pos = {0, 0});
  void settext(sf::String str);
  void setpos(sf::Vector2f pos);
  void updatetextpos();
  void draw_to_window(sf::RenderWindow &w);
  std::function<void(StateManager &s)> onclick;

protected:
  sf::Text text;
};
// Todo: create grid
// a grid holds UI components, and has different methods, to change their locations, depending on what's needed
// add functionality to the grid, so it can be scrollable

// Todo: create textbox component
class GridElements {
public:
  virtual void draw(sf::RenderWindow &w) = 0;
};
class TextBox : public GridElements {
  virtual void draw(sf::RenderWindow &w);

private:
  sf::Text textbox;
  bool isSelected;
  int width;
  int height;
  int limit;
};
// Todo: create namedtextbox component, which is basically a textbox which has a text component that is (by default) directly on top of textbox
class NamedTextBox : public GridElements {
public:
  virtual void draw(sf::RenderWindow &w);

private:
  TextBox tb;
  sf::Text label;
};
class NamedBox : public GridElements {
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

#endif
