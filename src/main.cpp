#include <SFML/Graphics.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <iostream>

// my headers
#include "../include/menu.hpp"
#include "../include/renderer.hpp"
#include "../include/statemanagement.hpp"

int main() {
    try {
        StateManager state_mngr;
        Renderer renderer{sf::VideoMode{800, 600}, "lolmacrogame"};
        Menu::MenuState *m = new Menu::MenuState;
        state_mngr.PushState(m);
        while (state_mngr.hasState()) {
            state_mngr.Update(renderer);
        }
        delete m;
    } catch (const char *err) {
        std::cout << "Oops, there was an error: " << err << std::endl;
        exit(1);
    }

    return 0;
}
