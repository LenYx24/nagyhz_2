#ifndef MENU_HPP
#define MENU_HPP

#include "UIcomponents.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>
#include <iostream>

namespace Menu {

class MenuButton : public Button {
public:
    MenuButton(
        Resources::Holder &h, sf::String str,
        std::function<void(StateManager &s)> onclick = [](StateManager &s) {
            std::cout << "not impl" << std::endl;
        });
};

class MenuState : public State {
public:
    MenuState() {}

    virtual void HandleEvents(StateManager &s, Renderer &renderer);
    virtual void Update(StateManager &s, Renderer &r);

protected:
    std::vector<Button> buttons;
    Resources::Holder h;
};
class ModeSelectionState : public MenuState {
public:
    ModeSelectionState();
};
class MainState : public MenuState {
public:
    MainState();
};

} // namespace Menu
#endif // !MENU
