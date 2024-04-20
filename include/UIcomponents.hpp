#ifndef UICOMPONENTS_HPP
#define UICOMPONENTS_HPP
#include <SFML/Graphics.hpp>
class Button : public sf::RectangleShape {
public:
    Button(sf::String str = "", sf::Vector2f pos = {0, 0});
    void settext(sf::String str);
    void setpos(sf::Vector2f pos);
    void updatetextpos();
    void draw_to_window(sf::RenderWindow &w);

protected:
    sf::Text text;
};
#endif
