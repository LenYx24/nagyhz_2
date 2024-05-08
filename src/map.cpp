#include "../include/map.hpp"

void Cell::setpos(sf::Vector2f pos) {
  indicies = pos;
}
void Cell::updateshape(sf::Vector2f mappos, sf::Vector2f cellsize, float margin) {
  shape.setFillColor(color);
  pos = {mappos.x + cellsize.x * indicies.x, mappos.y + cellsize.y * indicies.y};
  shape.setSize({cellsize.x - margin, cellsize.y - margin});
  shape.setPosition(pos);
  //std::cout << "pos: " << pos.x << " " << pos.y << std::endl;
}
void Cell::updateVision() {
  // update vision
}
bool Cell::canbuyitems() {
  return true;
}
void Cell::resetcolor(){
  sf::Color c = shape.getFillColor();
  c.a = 255;
  shape.setFillColor(c);
}
void Cell::sethighlighted() {
  sf::Color c = shape.getFillColor();
  c.a = 100;
  shape.setFillColor(c);
}
void Cell::draw(sf::RenderWindow &w) {
  w.draw(shape);
  for(size_t i = 0; i < entities.size(); i++){
    entities[i]->draw(w,pos);
  }
}
void Cell::setcolor(sf::Color c) {
  this->color = c;
}
Ground::Ground() {
  setcolor(sf::Color::Green);
}
River::River() {
  setcolor(sf::Color::Blue);
}
Wall::Wall() {
  setcolor(sf::Color{80, 60, 10});
}
Bush::Bush() {
  setcolor(sf::Color{0, 200, 0});
}
SpawnArea::SpawnArea() {
  setcolor(sf::Color::Yellow);
}
Map::Map(sf::Vector2f pos) {
  position = pos;
  cells = new Cell **[size.x];
  std::ifstream file("resources/map.txt");
  if (!file) {
    throw "error with opening map.txt";
  }
  for (size_t i = 0; i < size.x; i++) {
    cells[i] = new Cell *[size.y];
    std::string line;
    std::getline(file, line);
    //std::cout << line << std::endl;
    int lineindex = 0;
    for (size_t j = 0; j < size.y; j++) {
      //std::cout << "lineitem: " << line[lineindex] << std::endl;
      switch (line[lineindex]) {
      case 'o': {
        std::cout << "ground" << std::endl;
        cells[i][j] = new Ground{};
        break;
      }
      case '~': {
        std::cout << "river" << std::endl;
        cells[i][j] = new River{};
        break;
      }
      case '|': {
        std::cout << "wall" << std::endl;
        cells[i][j] = new Wall{};
        break;
      }
      case 'b': {
        std::cout << "bush" << std::endl;
        cells[i][j] = new Bush{};
        break;
      }
      case 't': {
        std::cout << "tower" << std::endl;
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Tower);
        break;
      }
      
      case 'n': {
        std::cout << "nexus" << std::endl;
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Nexus);
        break;
      }
      case 'j': {
        std::cout << "jungle" << std::endl;
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Camp);
        break;
      }
      case 'd': {
        std::cout << "drake" << std::endl;
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Camp);
        break;
      }
      case 'r': {
        std::cout << "jungle" << std::endl;
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Camp);
        break;
      }
      case 'y':
      case 'x': {
        std::cout << "spawn" << std::endl;
        cells[i][j] = new SpawnArea{};
        break;
      }
      default: {
        std::cout << "err: wrong symbol: " << line[lineindex] << std::endl;
        cells[i][j] = nullptr;
        break;
      }
      }
      if (cells[i][j] != nullptr) {
        cells[i][j]->setpos(sf::Vector2f{(float)i, (float)j});
        cells[i][j]->updateshape(pos, cellsize);
      }
      lineindex += 2; // skip semicolons
    }
  }
  file.close();
}
void Map::draw(sf::RenderWindow &w) {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->draw(w);
      }
    }
  }
}
void Map::update() {
  // update
}

Map::posindex Map::toposindex(sf::Vector2f pos) {
  return posindex{static_cast<size_t>(pos.x), static_cast<size_t>(pos.y)};
}
void Map::spawn(Entity *entity, sf::Vector2f pos) {
  posindex pindex = toposindex(pos);
  cells[pindex.i][pindex.j]->addentity(entity);
}
Map::~Map() {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      delete cells[i][j];
    }
  }
}

void Cell::addentity(Entity *entity) {
  entities.push_back(entity);
}
Cell *Map::getclickedcell(const int x, const int y) {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j]->contains(x, y)) {
        return cells[i][j];
      }
    }
  }
  std::cout << "cellnullptr" << std::endl;
  return nullptr;
}
bool Cell::contains(const int x, const int y) {
  return shape.getGlobalBounds().contains(x, y);
}

Entity* Cell::getentitiyclicked(const int x, const int y){
  for(size_t i = 0; i < entities.size(); i++){
    if(entities[i]->clicked(x,y))return entities[i];
  }
  return nullptr;
}
std::vector<Cell*> Map::getnearbycells(sf::Vector2f pos, int distance){
  std::vector<Cell *> around;
  std::cout << "searching for nearby cells" << std::endl;
  std::cout << "pos: " << pos.x << ":" << pos.y << std::endl;
  for(int i = pos.x-distance; i <= pos.x+distance; i++){
    std::cout << "i: " << i << std::endl;
    if(inboundsrow(i)){
      for(int j = pos.y-distance; j <= pos.y+distance; j++){
        std::cout << "j: " << j << std::endl;
        if(inboundscol(j) && cells[i][j] != nullptr){
          std::cout << "cell added at: i: " << i << " j: " << j << std::endl;
          around.push_back(cells[i][j]);
        }
      }
    }
    
  }
  return around;
}
void Map::setselectednearbycells(Champion *c){
  sf::Vector2f index = c->getcell()->getindex();
  std::vector<Cell *> nearbycells = getnearbycells(index);
  for(size_t i = 0; i < nearbycells.size(); i++){
    nearbycells[i]->setselected();
  }
}
void Cell::setselected(){
  sethighlighted();
  selected = true;
}

void Map::resetcolors(){
  for(size_t i = 0; i < size.x; i++){
    for(size_t j = 0; j < size.y; j++){
      cells[i][j]->resetcolor();
    }
  }
}