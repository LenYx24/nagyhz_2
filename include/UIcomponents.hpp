#ifndef UI_COMPONENTS
#define UI_COMPONENTS

#include <SFML/Graphics.hpp>
#include <functional>
#include <iostream>
#include <vector>

#include "resources.hpp"
#include "statemanagement.hpp"

namespace UI {

class GridElement {
public:
  virtual ~GridElement() = default;
  virtual void draw(sf::RenderWindow& window) = 0;
  virtual void set_position(sf::Vector2f pos) = 0;
  [[nodiscard]] virtual bool contains(int x, int y)const=0;
  virtual sf::Vector2f get_size() = 0;
};

class Button : public GridElement {
public:
  Button() = default;
  explicit Button(
      const sf::String& text, std::function<void()> onclick = []() { std::cout << "onclick not implemented yet" << std::endl;},
      sf::Vector2f pos = {0, 0});
  void set_position(sf::Vector2f pos) override;
  void update_text_position();
  void draw_to_window(sf::RenderWindow &window);
  bool contains(int x, int y)const override;

  void draw(sf::RenderWindow& window) override;
  sf::Vector2f get_size() override{return shape.getSize();}
  sf::FloatRect get_global_bounds() const {return shape.getGlobalBounds();}
  void onclick_here(const sf::Event &e);
  void set_onclick(std::function<void()> onclick_);

protected:
  sf::RectangleShape shape;
  sf::Text text;
  std::function<void()> onclick;
};
class TextBox : public GridElement {
public:
  TextBox(const std::string& label,Resources::Holder &holder, sf::Vector2f pos = {0, 0}, const std::string& text_default="");
  void draw(sf::RenderWindow& window) override;
  void set_position(sf::Vector2f pos)override{shape.setPosition(pos);}
  sf::Vector2f get_size()override{return shape.getSize();}
  bool contains(int x, int y)const override;
  inline sf::FloatRect get_global_bounds() const {
    return shape.getGlobalBounds();
  }
  void set_selected(bool s){is_selected = s;}
  bool get_is_selected()const{return is_selected;}
  void add_char(char c);
  void remove_char();
  std::string get_text()const{return text.getString();}
private:
  sf::Text text;
  sf::RectangleShape shape;
  sf::Text label;
  bool is_selected = false;
};
class Grid {
public:
  Grid(sf::Vector2f start_pos, sf::Vector2f margin_, sf::Vector2f direction_ = {1, 0});
  void set_elements(std::vector<GridElement *> elements_);
  void set_elements_pos();
  [[nodiscard]] bool contains(int x, int y)const;
  [[nodiscard]] sf::FloatRect get_global_bounds() const;
private:
  std::vector<GridElement *> elements;
  sf::Vector2f start_pos;
  sf::Vector2f margin;
  sf::Vector2f direction;
};
class NamedBox : public GridElement {
public:
  explicit NamedBox(Resources::Holder &h) {label.setFont(h.get(Resources::Type::FONT));}
  NamedBox(const std::string& label, sf::RectangleShape frame, Resources::Holder &h);
  void set_label(const std::string& l);
  void set_position(sf::Vector2f pos) override;
  void set_char_size(unsigned size);
  void set_label_color(const sf::Color &c);
  sf::FloatRect get_global_bounds() const {
    return frame.getGlobalBounds();
  }
  bool contains(int x, int y)const override;
  sf::Vector2f get_size() override;
  void draw(sf::RenderWindow& window)override;

protected:
  sf::RectangleShape frame;
  sf::Text label;
};

} // namespace UI

#endif
