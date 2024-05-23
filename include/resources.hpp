#ifndef RESOURCES_HPP
#define RESOURCES_HPP
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <map>
#include <memory>

namespace Resources {
/**
 * @brief the types of resources there are
 */
enum class Type { FONT };
/**
 * @brief the class which holdes the resources for the application
 */
class Holder {
public:
  /**
   * @brief loads the given resources
   * @param type the type of the resource
   * @param filename the path to the resource
   */
    void load(Type type, const sf::String &filename);
    sf::Font &get(Type type);

private:
    std::map<Type, std::unique_ptr<sf::Font>> resources;
};

} // namespace Resources
#endif
