#ifndef MENU_HPP
#define MENU_HPP

#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Menu {

class Button : public sf::RectangleShape {
public:
    Button(sf::String str = "", sf::Vector2f pos = {0, 0});
    void settext(sf::String str = "") { text.setString(str); }
    void setpos(sf::Vector2f pos) {
        setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
        this->setPosition(pos);
        settextpos();
    }
    void settextpos() {
        text.setPosition(getPosition().x - text.getLocalBounds().width / 2.f,
                         getPosition().y - text.getLocalBounds().height / 1.5f);
    }
    void draw_to_window(sf::RenderWindow &w) {
        w.draw(*this);
        sf::Font font;
        if (!font.loadFromFile("./fonts/Roboto.ttf")) {
            throw "Fonts could not be loaded";
        }
        text.setFont(font);
        settextpos();
        w.draw(text);
    }

private:
    sf::Text text;
};

class MenuState : public State {
public:
    MenuState();
    virtual ~MenuState();

    virtual void Update(StateManager &s, Renderer &r);

private:
    std::vector<Button> buttons;
};

} // namespace Menu
#endif // !MENU
