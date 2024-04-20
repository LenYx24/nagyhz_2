#ifndef RENDERER_HPP
#define RENDERER_HPP
#include <SFML/Graphics.hpp>
class Renderer {
public:
    Renderer(sf::VideoMode mode, sf::String title);

    ~Renderer();
    bool PollEvent(sf::Event &e);
    inline sf::RenderWindow &getWindow() { return window; }

private:
    sf::RenderWindow window;
    bool GUI;
};
#endif
