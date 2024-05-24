#include "../include/resources.hpp"

using namespace Resources;
void Holder::load(Type type, const sf::String &filename) {
    std::unique_ptr<sf::Font> font(new sf::Font);
    if (!font->loadFromFile(filename)) {
        throw std::invalid_argument("there was an error loading font");
    }
    resources.insert(std::make_pair(type, std::move(font)));
}

sf::Font &Holder::get(Type type) { return *resources.find(type)->second; }
