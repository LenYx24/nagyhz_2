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
class Grid {
public:
  virtual void drawgrid() = 0;

protected:
  std::vector<sf::Drawable *> elements;
  sf::Vector2f length; // defines how many elements should be in a row / column
};
// Todo: create textbox component
class TextBox {
private:
  sf::Text textbox;
  bool isSelected;
  int width;
  int height;
  int limit;
};
// Todo: create namedtextbox component, which is basically a textbox which has a text component that is (by default) directly on top of textbox
class NamedTextBox {
private:
  TextBox tb;
  sf::Text label;
};

#endif
