#include "../include/menu.hpp"

namespace Menu {

MenuState::MenuState() {
    buttons.push_back(Button{"Start"});
    buttons.push_back(Button{"Settings"});
    buttons.push_back(Button{"Exit"});
    // Todo: put this into its own grid class
    float marginy = 20 + buttons[0].getSize().y / 2.f;
    sf::Vector2f pos{800 / 2.f, marginy};
    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].setPosition(pos);
        pos.y += buttons[i].getSize().y / 2.f + marginy;
    }
}
Button::Button(sf::String str, sf::Vector2f pos) {

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
MenuState::~MenuState() {}
void MenuState::Update(StateManager &s, Renderer &renderer) {
    // update
    for (sf::Event event = sf::Event{}; renderer.PollEvent(event);) {
        if (event.type == sf::Event::Closed) {
            s.exit();
        } else if (event.type == sf::Event::MouseButtonPressed) {
            std::cout << "[[INFO]] mouse clicked" << std::endl;
        }
    }
    sf::RenderWindow &w = renderer.getWindow();
    sf::Color background_color = sf::Color(220, 225, 222);
    w.clear(background_color);
    for (size_t i = 0; i < 3; i++) {
        buttons[i].draw_to_window(w);
    }

    w.display();
}
} // namespace Menu
