#include "../include/map.hpp"

void Ground::canbuyitems() {}
void Ground::sethighlighted() {}
void Cell::setpos(sf::Vector2f pos) {
  position = pos;
}
Ground::Ground() {
  canmove = true;
  color = sf::Color::Green;
}
Map::Map() {
  cells = new Cell**[size.x];
  for (size_t i = 0; i < size.x; i++) {
    cells[i] = new Cell*[size.y];
    for (size_t j = 0; j < size.y; j++) {
      cells[i][j] = new Ground{};
      cells[i][j]->setpos(sf::Vector2f{(float)i, (float)j});
    }
  }
}
void Map::draw() {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      // draw
    }
  }
}
void Map::update() {
  // update
}
Map::~Map() {}
