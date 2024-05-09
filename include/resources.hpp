#ifndef RESOURCES_HPP
#define RESOURCES_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <memory>

// Todo: szedjem külön
namespace Resources {
enum class Type { FONT };

// Todo: do it with templates
class Holder {
public:
    void load(Type type, const sf::String &filename);
    sf::Font &get(Type type);

private:
    std::map<Type, std::unique_ptr<sf::Font>> resources;
};

} // namespace Resources
#endif
