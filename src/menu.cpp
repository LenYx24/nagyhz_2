#include "../include/menu.hpp"

namespace Menu {
MenuButton::MenuButton(Resources::Holder &h, sf::String str) : Button(str) {
    // menu button specific override settings
    text.setCharacterSize(15);
    text.setFont(h.get(Resources::Type::FONT));
}
MenuState::MenuState() {
    h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
    buttons.push_back(MenuButton{h, "Start"});
    buttons.push_back(MenuButton{h, "Settings"});
    buttons.push_back(MenuButton{h, "Exit"});
    // Todo: put this into its own grid class
    float marginy = 20 + buttons[0].getSize().y / 2.f;
    sf::Vector2f pos{800 / 2.f, marginy};
    for (size_t i = 0; i < buttons.size(); i++) {
        buttons[i].setPosition(pos);
        pos.y += buttons[i].getSize().y / 2.f + marginy;
    }
}

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
