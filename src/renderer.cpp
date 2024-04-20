#include "../include/renderer.hpp"

Renderer::Renderer(sf::VideoMode mode, sf::String title)
    : window(sf::RenderWindow{mode, title, sf::Style::Default,
                              sf::ContextSettings{0, 0, 2}}) {}
Renderer::~Renderer() { window.close(); }

bool Renderer::PollEvent(sf::Event &e) { return window.pollEvent(e); }
