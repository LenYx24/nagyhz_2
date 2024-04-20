#ifndef MENU_HPP
#define MENU_HPP

#include "UIcomponents.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

namespace Menu {

class MenuButton : public Button {
public:
    MenuButton(Resources::Holder &h, sf::String str = "");
};

class MenuState : public State {
public:
    MenuState();
    virtual void Update(StateManager &s, Renderer &r);

private:
    std::vector<Button> buttons;
    Resources::Holder h;
};

} // namespace Menu
#endif // !MENU
