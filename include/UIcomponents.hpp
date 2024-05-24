#ifndef UI_COMPONENTS
#define UI_COMPONENTS

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>

#include "resources.hpp"
#include "statemanagement.hpp"
namespace UI {
/**
 * @brief the base class for grid elements
 */
class GridElement {
public:
  virtual ~GridElement() = default;
  /**
   * @brief tells the gridelement to draw itself to the window
   * @param window
   */
  virtual void draw(sf::RenderWindow& window) = 0;
  /**
   * @brief set's the position of the grid element relative to the window
   * @param pos
   */
  virtual void set_position(sf::Vector2f pos) = 0;
  /**
   * @brief checks if the given coordinates are inside the grid element
   * @param x x coordinate
   * @param y y coordinate
   * @return true if they're inside, false if not
   */
  [[nodiscard]] virtual bool contains(int x, int y)const=0;
  /**
   * @brief get's the size of the grid element
   * @return the size
   */
  virtual sf::Vector2f get_size() = 0;
};
/**
 * @brief the button class which implements a shape with some text on it, with an onclick method
 */
class Button : public GridElement {
public:
  Button() = default;
  /**
   * @brief constructs a button with the given params
   * @param text the text on the button
   * @param onclick the method to be called when the button gets clicked on
   * @param pos the position relative to the window of the button
   */
  explicit Button(
      const sf::String& text,
      std::function<void()> onclick = []() { std::cout << "onclick not implemented yet" << std::endl;},
      sf::Vector2f pos = {0, 0});
  bool contains(int x, int y)const override;
  void draw(sf::RenderWindow& window) override;
  sf::Vector2f get_size() override{return shape.getSize();}
  void set_position(sf::Vector2f pos) override;
  /**
   * @brief updates the texts position relative to the shape
   */
  void update_text_position();
  /**
   * @brief gets the global bounds of the buttons shape
   * @return the global bounds
   */
  sf::FloatRect get_global_bounds() const {return shape.getGlobalBounds();}
  /**
   * @brief the method to call to check if the button was clicked, if so then it calls his onclick
   * @param event the event
   */
  void onclick_here(const sf::Event &event);

protected:
  sf::RectangleShape shape;
  sf::Text text;
  std::function<void()> onclick;
};
/**
 * @brief the textbox element, which is a rectangle where text you can input text into
 */
class TextBox : public GridElement {
public:
  /**
   * @brief constructs a textbox with the given params
   * @param label the name of the textbox, this is placed outside the rectangle, showing what the textbox is for
   * @param holder the resources holder
   * @param pos the position relative to the window
   * @param text_default the default text inside the rectangle
   */
  TextBox(const std::string& label,
          Resources::Holder &holder,
          sf::Vector2f pos = {0, 0},
          const std::string& text_default="");
  void draw(sf::RenderWindow& window) override;
  void set_position(sf::Vector2f pos)override{shape.setPosition(pos);}
  sf::Vector2f get_size()override{return shape.getSize();}
  bool contains(int x, int y)const override;
  /**
   * @brief gets the global bounds of the shape
   * @return the rectangle
   */
  sf::FloatRect get_global_bounds() const {return shape.getGlobalBounds();}
  /**
   * @brief set's the textbox selected, that means it takes in input text
   * @param s true if the textbox got selected, false otherwise
   */
  void set_selected(bool s){is_selected = s;}
  /**
   * @brief checks if the textbox is selected
   * @return true if it is selected, false otherwise
   */
  bool get_is_selected()const{return is_selected;}
  /**
   * @brief adds the given character to the rectangles text
   * @param c the char to add
   */
  void add_char(char c);
  /**
   * @brief removes the last character from the text
   */
  void remove_char();
  /**
   * @brief gets the textbox's inside text
   * @return the text
   */
  std::string get_text()const{return text.getString();}
private:
  sf::Text text;
  sf::RectangleShape shape;
  sf::Text label;
  bool is_selected = false;
};
/**
 * @brief the grid holds multiple grid elements, and places them in a given way
 */
class Grid {
public:
  /**
   * @brief constructs a grid with the given params
   * @param start_pos the start position of the grid relative to the window
   * @param margin_ the margin between grid elements
   * @param direction_ the direction of the grid, it is the vector the grid goes towards while placing elements
   */
  Grid(sf::Vector2f start_pos, sf::Vector2f margin_, sf::Vector2f direction_ = {1, 0});
  /**
   * @brief set's this grid's elements
   * @param elements_
   */
  void set_elements(std::vector<GridElement *> elements_);
  /**
   * @brief set's the elements position by calculating it with the its properties
   */
  void set_elements_pos();
  /**
   * @brief checks if the given coordinates are inside the grid
   * @param x x coordinate
   * @param y y coordinate
   * @return true if they're inside, false if not
   */
  [[nodiscard]] bool contains(int x, int y)const;
  /**
   * @brief gets the global bounds of the rectangle
   * @return the rectangle
   */
  [[nodiscard]] sf::FloatRect get_global_bounds() const;
private:
  std::vector<GridElement *> elements;
  sf::Vector2f start_pos;
  sf::Vector2f margin;
  sf::Vector2f direction;
};
/**
 * @brief the named box, which is a grid element that holds a shape and a text inside of it
 */
class NamedBox : public GridElement {
public:
  /**
   * @brief constructs a named box
   * @param holder the resources holder
   */
  explicit NamedBox(Resources::Holder &holder);
  /**
   * @brief constructs a named box with the given parameters
   * @param label the text inside the shape
   * @param frame the shape
   * @param holder the resources holder
   */
  NamedBox(const std::string& label, sf::RectangleShape frame, Resources::Holder &holder);
  void set_position(sf::Vector2f pos) override;
  bool contains(int x, int y)const override;
  sf::Vector2f get_size() override;
  void draw(sf::RenderWindow& window)override;
  /**
   * @brief set's the new text of the named box
   * @param label_text
   */
  void set_label(const std::string& label_text);
  /**
   * @brief set's the character size of the named box
   * @param size the new size
   */
  void set_char_size(unsigned size);
  /**
   * @brief set's the label color
   * @param c the color to use
   */
  void set_label_color(const sf::Color &c);
  /**
   * @brief gets the global bounds of the shape
   * @return the rectangle
   */
  sf::FloatRect get_global_bounds() const {
    return frame.getGlobalBounds();
  }

protected:
  sf::RectangleShape frame;
  sf::Text label;
};

} // namespace UI

#endif
