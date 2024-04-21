#include "../include/gameobjects.hpp"

Entity::Entity(std::string name) : name(name) {}

void Champion::spawn() {
  // spawns the champion
}
void Champion::die() {
  // champions dies
}

void Champion::update_vision() {}
void Champion::readfromstring(std::string &line, const char delimiter) {
  // Todo: read champ data
  name = line.substr(0, line.find(delimiter));
}
