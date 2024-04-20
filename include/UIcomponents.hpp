#ifndef UICOMPONENTS_HPP
#define UICOMPONENTS_HPP
#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <functional>
class Button : public sf::RectangleShape {
public:
    Button() {}
    Button(
        sf::String str,
        std::function<void(StateManager &s)> onclick_ =
            [](StateManager &s) {
                std::cout << "onclick not implemented yet" << std::endl;
            },
        sf::Vector2f pos = {0, 0});
    void settext(sf::String str);
    void setpos(sf::Vector2f pos);
    void updatetextpos();
    void draw_to_window(sf::RenderWindow &w);
    std::function<void(StateManager &s)> onclick;

protected:
    sf::Text text;
};
#endif
