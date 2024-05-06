#include "../include/map.hpp"

void Ground::canbuyitems() {}
void Ground::sethighlighted() {}
void Cell::setpos(sf::Vector2f pos) {
  position = pos;
}
void Cell::updateshape(sf::Vector2f mappos, sf::Vector2f cellsize, float margin) {
  shape.setFillColor(color);
  sf::Vector2f _pos = {mappos.x + cellsize.x * position.x , mappos.y + cellsize.y * position.y};
  shape.setSize({cellsize.x- margin,cellsize.y-margin});
  shape.setPosition(_pos);
  std::cout << "pos: " << _pos.x << " " << _pos.y << std::endl;
}

void Cell::draw(sf::RenderWindow& w){
  w.draw(shape);
}
Ground::Ground() {
  canmove = true;
  color = sf::Color::Green;
}
Map::Map(sf::Vector2f pos) {
  position = pos;
  cells = new Cell**[size.x];
  for (size_t i = 0; i < size.x; i++) {
    cells[i] = new Cell*[size.y];
    for (size_t j = 0; j < size.y; j++) {
      cells[i][j] = new Ground{};
      cells[i][j]->setpos(sf::Vector2f{(float)i, (float)j});
      cells[i][j]->updateshape(pos,cellsize);
    }
  }
}
void Map::draw(sf::RenderWindow& w) {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      // draw
      cells[i][j]->draw(w);
    }
  }
}
void Map::update() {
  // update
}

  Map::posindex Map::toposindex(sf::Vector2f pos){
    return posindex{static_cast<size_t>(pos.x),static_cast<size_t>(pos.y)};
  }
  void Map::spawn(Entity *entity, sf::Vector2f pos){
    posindex pindex = toposindex(pos);
    cells[pindex.i][pindex.j]->addentity(entity);
  }
Map::~Map() {}

void Cell::addentity(Entity *entity){
  entities.push_back(entity);
}