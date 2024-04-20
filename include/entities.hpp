#ifndef ENTITIES_HPP
#define ENTITIES_HPP
#include <SFML/Graphics.hpp>
#include <iostream>
class Entity {
public:
    Entity(std::string name = "");

private:
    const std::string name;

    // todo add sprite tag for entity
};
#endif
