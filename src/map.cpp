#include "../include/map.hpp"

void Cell::setpos(sf::Vector2f index) {
  indicies = index;
}
void Cell::updateshape(sf::Vector2f mappos, sf::Vector2f cellsize, float margin) {
  shape.setFillColor(color);
  pos = {mappos.x + cellsize.x * indicies.x, mappos.y + cellsize.y * indicies.y};
  shape.setSize({cellsize.x - margin, cellsize.y - margin});
  shape.setPosition(pos);
  update_entities_shape(pos);
}
void Cell::update_entities_shape(sf::Vector2f mappos){
  for(size_t i = 0; i < entities.size(); i++){
    entities[i]->update_shape_pos(mappos);
  }
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
    if(entities[i] != nullptr)
      entities[i]->draw(w);
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
  set_shop(true);
}
Map::Map(sf::Vector2f pos) {
  position = pos;
  //cells = new Cell **[size.x];
  std::ifstream file("resources/map.txt");
  if (!file) {
    throw "error with opening map.txt";
  }
  for (size_t i = 0; i < size.x; i++) {
    std::string line;
    std::getline(file, line);
    int lineindex = 0;
    for (size_t j = 0; j < size.y; j++) {
      switch (line[lineindex]) {
      case 'o': {
        cells[i][j] = new Ground{};
        break;
      }
      case '~': {
        cells[i][j] = new River{};
        break;
      }
      case '|': {
        cells[i][j] = new Wall{};
        break;
      }
      case 'b': {
        cells[i][j] = new Bush{};
        break;
      }
      // the blue side turret
      case 't': {
        cells[i][j] = new Ground{};
        Tower *tower = new Tower;
        tower->set_side(Side::BLUE);
        cells[i][j]->addentity(tower);
        break;
      }
      // the red side turret
      case 'z': {
        cells[i][j] = new Ground{};
        Tower *tower = new Tower;
        tower->set_side(Side::RED);
        cells[i][j]->addentity(tower);
        break;
      }
      case 'n': {
        cells[i][j] = new Ground{};
        Nexus *nexus = new Nexus;
        nexus->set_side(Side::BLUE);
        cells[i][j]->addentity(nexus);
        break;
      }
      case 'm': {
        cells[i][j] = new Ground{};
        Nexus *nexus = new Nexus;
        nexus->set_side(Side::RED);
        cells[i][j]->addentity(nexus);
        break;
      }
      // sima jungle camp
      case 'j': {
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Camp);
        break;
      }
      // baron
      case 'a': {
        cells[i][j] = new Ground{};
        Camp *baron = new Camp;
        baron->set_name("baron");
        baron->set_color(sf::Color{130,50,170});
        // Todo: set it's effect to minionbuff
        cells[i][j]->addentity(baron);
        break;
      }
      // drake
      case 'd': {
        cells[i][j] = new Ground{};
        cells[i][j]->addentity(new Drake);
        break;
      }
      // redbuff
      case 'r': {
        cells[i][j] = new Ground{};
        Camp *c = new Camp;
        c->set_name("red buff");
        c->set_color(sf::Color{250,50,50});
        c->setEffect(Effect{10,0});
        cells[i][j]->addentity(c);
        break;
      }
      // bluebuff
      case 'c': {
        cells[i][j] = new Ground{};
        Camp *c = new Camp;
        c->set_name("blue buff");
        c->set_color(sf::Color{50,50,250});
        c->setEffect(Effect{0,10});
        cells[i][j]->addentity(c);
        break;
      }
      case 'x':{
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
void Map::reset_cell_selections(){
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->unselect();
        cells[i][j]->resetcolor();
      }
    }
  }
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
bool Cell::should_update_vision_around(Side current_side){
  for(size_t i = 0; i < entities.size(); i++){
    if(entities[i]->get_side() == current_side){
      return true;
    }
  }
  return false;
}
void Map::update_vision(Side current_side){
  // first reset every cells shapes opacity to default
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      bool should_update = cells[i][j]->should_update_vision_around(current_side);
      if(should_update){
        // Todo: get nearby cells, and set their shapes opacity
      }
    }
  }
}
void Cell::addentity(Entity *entity) {
  entities.push_back(entity);
  update_entities_shape(shape.getPosition());
}
void Cell::remove_entity(Entity *entity){
  std::vector<Entity *>::iterator element = std::find(entities.begin(),entities.end(),entity);
  entities.erase(element);
}
Cell *Map::getclickedcell(const int x, const int y) {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j]->contains(x, y)) {
        return cells[i][j];
      }
    }
  }
  std::cout << "x:" <<x << "y: " << y << "  !!cellnullptr" << std::endl;
  return nullptr;
}
bool Cell::contains(const int x, const int y) {
  return shape.getGlobalBounds().contains(static_cast<float>(x),static_cast<float>(y));
}
Entity* Cell::getentitiyclicked(const int x, const int y){
  // need to start iterating from the back, because of how the entities are drawn to the screen, 
  //the last one is drawn on top of the other ones
  for(int i = static_cast<int>(entities.size())-1; i >=0; i--){
    if(entities[i]->clicked(x,y))return entities[i];
  }
  return nullptr;
}
std::vector<Cell*> Map::getnearbycells(sf::Vector2f pos, int distance){
  std::vector<Cell *> around;
  sf::Vector2 pos_ = {static_cast<int>(pos.x),static_cast<int>(pos.y)};
  for(int i = pos_.x-distance; i <= pos_.x+distance; i++){
    if(inboundsrow(i)){
      for(int j = pos_.y-distance; j <= pos_.y+distance; j++){
        if(inboundscol(j) && cells[i][j] != nullptr){
          around.push_back(cells[i][j]);
        }
      }
    }
    
  }
  return around;
}
template<typename P>
void Map::setselectednearbycells(Champion *c, P pred){
  sf::Vector2f index = c->get_simulation_cell()->getindex();
  std::vector<Cell *> nearbycells = getnearbycells(index);
  for(size_t i = 0; i < nearbycells.size(); i++){
    if(pred(nearbycells[i]))
      nearbycells[i]->setselected();
  }
}
void Map::select_accessible_cells(Champion *c){
  setselectednearbycells(c,[](Cell *c){return c->canmovehere();});
}
void Map::select_wardable_cells(Champion *c){
  setselectednearbycells(c,[](Cell *c){return c->can_ward_here();});
}
void Map::select_attackable_entities(Champion *c){
  setselectednearbycells(c,[](Cell *c){return c->can_attack_entity();});
}
void Cell::unselect(){
  selected = false;
}
void Cell::setselected(){
  sethighlighted();
  selected = true;
}
void Map::move(Entity *entity, sf::Vector2f from, sf::Vector2f to){
  posindex from_index = toposindex(from); 
  posindex to_index = toposindex(to);
  cells[from_index.i][from_index.j]->remove_entity(entity);
  cells[to_index.i][to_index.j]->addentity(entity);
  //entity->update_shape_pos(position);
}