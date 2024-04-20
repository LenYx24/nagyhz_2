#include "../include/UIcomponents.hpp"
Button::Button(sf::String str, sf::Vector2f pos) {
    // default button settings
    text.setString(str);
    text.setFillColor({220, 225, 223});
    text.setCharacterSize(20);
    setOutlineThickness(1);
    setOutlineColor({31, 36, 33});
    setFillColor({33, 104, 105});
    setSize({500, 120});
    text.setOrigin(
        {text.getLocalBounds().width / 2, text.getLocalBounds().height / 2});
    setpos(pos);
}

void Button::settext(sf::String str) { text.setString(str); }
void Button::setpos(sf::Vector2f pos) {
    setOrigin(getLocalBounds().width / 2.f, getLocalBounds().height / 2.f);
    this->setPosition(pos);
    updatetextpos();
}
void Button::updatetextpos() {
    text.setPosition(getPosition().x - text.getLocalBounds().width / 2.f,
                     getPosition().y - text.getLocalBounds().height / 1.5f);
}
void Button::draw_to_window(sf::RenderWindow &w) {
    w.draw(*this);
    updatetextpos();
    w.draw(text);
}
