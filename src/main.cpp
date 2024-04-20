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
        state_mngr.PushState(new Menu::MainState);
        while (state_mngr.hasState()) {
            state_mngr.HandleEvents(renderer);
            state_mngr.Update(renderer);
        }
    } catch (const char *err) {
        std::cout << "Oops, there was an error: " << err << std::endl;
        exit(1);
    }

    return 0;
}
