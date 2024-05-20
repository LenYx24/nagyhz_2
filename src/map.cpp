#include "../include/map.hpp"

void Cell::set_position(sf::Vector2f pos_) {
  indicies = pos_;
}
void Cell::update_shape(sf::Vector2f map_position, sf::Vector2f cell_size, float margin) {
  shape.setFillColor(color);
  pos = {map_position.x + cell_size.x * indicies.x, map_position.y + cell_size.y * indicies.y};
  shape.setSize({cell_size.x - margin, cell_size.y - margin});
  shape.setPosition(pos);
  update_entities_shape(pos);
}
void Cell::update_entities_shape(sf::Vector2f mappos){
  for(size_t i = 0; i < entities.size(); i++){
    entities[i]->update_shape_pos(mappos);
  }
}
void Cell::reset_selection_color(){
  sf::Color c = shape.getFillColor();
  if(c.a != no_vision_opacity){
    c.a = has_vision_opacity;
  }
  shape.setFillColor(c);
}
void Cell::reset_vision_color(){
  sf::Color c = shape.getFillColor();
  if(c.a != selected_opacity){
    c.a = has_vision_opacity;
  }
  shape.setFillColor(c);
}

void Cell::set_highlighted() {
  sf::Color c = shape.getFillColor();
  c.a = selected_opacity;
  shape.setFillColor(c);
}
void Cell::draw(sf::RenderWindow &w) {
  w.draw(shape);
  for(auto & entitie : entities){
    if(entitie != nullptr ) // && shape.getFillColor().a == has_vision_opacity
      entitie->draw(w);
  }
}
void Cell::set_color(sf::Color c) {
  this->color = c;
}
Ground::Ground() {
  set_color(sf::Color::Green);
}
River::River() {
  set_color(sf::Color::Blue);
}
Wall::Wall() {
  set_color(sf::Color{80, 60, 10});
}
Bush::Bush() {
  set_color(sf::Color{0, 200, 0});
}
SpawnArea::SpawnArea() {
  set_color(sf::Color::Yellow);
  set_shop(true);
}
Map::Map(sf::Vector2f pos) {
  position = pos;
  std::string map_filename = "resources/map.txt";
  std::ifstream file(map_filename);
  if (!file) {
    throw std::runtime_error("error with opening "+map_filename);
  }
  for (size_t i = 0; i < size.x; i++) {
    std::string line;
    std::getline(file, line);
    size_t line_index = 0;
    for (size_t j = 0; j < size.y; j++) {
      switch (line[line_index]) {
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
      // the red side turret
      case 't': {
        cells[i][j] = new Ground{};
        Tower *tower = new Tower;
        tower->set_side(Side::RED);
        cells[i][j]->add_entity(tower);
        break;
      }
      // the blue side turret
      case 'z': {
        cells[i][j] = new Ground{};
        Tower *tower = new Tower;
        tower->set_side(Side::BLUE);
        cells[i][j]->add_entity(tower);
        break;
      }
      case 'n': {
        cells[i][j] = new Ground{};
        Nexus *nexus = new Nexus;
        nexus->set_side(Side::RED);
        cells[i][j]->add_entity(nexus);
        break;
      }
      case 'm': {
        cells[i][j] = new Ground{};
        Nexus *nexus = new Nexus;
        nexus->set_side(Side::BLUE);
        cells[i][j]->add_entity(nexus);
        break;
      }
      // sima jungle camp
      case 'j': {
        cells[i][j] = new Ground{};
        cells[i][j]->add_entity(new Camp);
        break;
      }
      // baron
      case 'a': {
        cells[i][j] = new Ground{};
        Camp *baron = new Camp;
        baron->set_name("baron");
        baron->set_color(sf::Color{130,50,170});
        // Todo: set it's effect to minionbuff
        cells[i][j]->add_entity(baron);
        break;
      }
      // drake
      case 'd': {
        cells[i][j] = new Ground{};
        cells[i][j]->add_entity(new Drake);
        break;
      }
      // redbuff
      case 'r': {
        cells[i][j] = new Ground{};
        Camp *c = new Camp;
        c->set_name("red buff");
        c->set_color(sf::Color{250,50,50});
        c->setEffect(Effect{10,0});
        cells[i][j]->add_entity(c);
        break;
      }
      // bluebuff
      case 'c': {
        cells[i][j] = new Ground{};
        Camp *c = new Camp;
        c->set_name("blue buff");
        c->set_color(sf::Color{50,50,250});
        c->setEffect(Effect{0,10});
        cells[i][j]->add_entity(c);
        break;
      }
      case 'x':{
        cells[i][j] = new SpawnArea{};
        break;
      }
      default: {
        std::cout << "err: wrong symbol: " << line[line_index] << std::endl;
        cells[i][j] = nullptr;
        break;
      }
      }
      if (cells[i][j] != nullptr) {
        cells[i][j]->set_position(sf::Vector2f{(float)i, (float)j});
        cells[i][j]->update_shape(pos, cellsize);
      }
      line_index += 2; // skip semicolons
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
void Map::reset_cell_selections(){
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->unselect();
        cells[i][j]->reset_selection_color();
      }
    }
  }
}
void Map::reset_cell_vision(){
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->reset_vision_color();
      }
    }
  }
}
void Map::disable_vision() {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->reset_vision_color();
      }
    }
  }
}
Map::posindex Map::toposindex(sf::Vector2f pos) {
  return posindex{static_cast<size_t>(pos.x), static_cast<size_t>(pos.y)};
}
void Map::spawn(Entity *entity, sf::Vector2f pos) {
  posindex pindex = toposindex(pos);
  cells[pindex.i][pindex.j]->add_entity(entity);
}
void Map::de_spawn(Entity *entity, sf::Vector2f pos){
  posindex pindex = toposindex(pos);
  cells[pindex.i][pindex.j]->remove_entity(entity);
}
Map::~Map() {
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      if(cells[i][j] != nullptr)
        delete cells[i][j];
    }
  }
}
Cell::~Cell(){
  for(size_t i = 0; i < entities.size(); i++){
    if(entities[i] != nullptr)delete entities[i];
  }
}
bool Cell::should_update_vision_around(Side current_side){
  for(size_t i = 0; i < entities.size(); i++){
    if(entities[i]->get_side() == current_side && entities[i]->gives_vision()){
      return true;
    }
  }
  return false;
}
sf::Uint8 Cell::has_vision_opacity = 255;
sf::Uint8 Cell::no_vision_opacity = 50;
sf::Uint8 Cell::selected_opacity = 100;
void Map::update_vision(){
  for(size_t i = 0; i < size.x; i++){
    for(size_t j = 0; j < size.y; j++){
      cells[i][j]->set_vision(false);
    }
  }
  for (size_t i = 0; i < size.x; i++) {
    for (size_t j = 0; j < size.y; j++) {
      bool should_update = cells[i][j]->should_update_vision_around(vision_side);
      if(should_update){
        sf::Vector2f pos = {static_cast<float>(i),static_cast<float>(j)};
        std::vector<Cell *> nearby_cells = getnearbycells(pos);
        for(Cell* cell: nearby_cells){
          cell->set_vision(true);
        }
      }
    }
  }
}
Entity *Cell::get_attackable_entity(Side side_){
  for(auto & entitie : entities){
    if(entitie->get_side() != side_)return entitie;
  }
  return nullptr;
}
void Map::check_game_end(){
  for(Entity *nexus: nexuses){
    if(!nexus->isAlive()){
      game_end = true;
    }
  }
}
void Cell::set_vision(bool has_vision){
  sf::Color shape_color = shape.getFillColor();
  if(shape_color.a != selected_opacity){
    if(has_vision){
      shape_color.a = has_vision_opacity;
    }
    else{
      shape_color.a = static_cast<sf::Uint8>(no_vision_opacity);
    }
  }
  shape.setFillColor(shape_color);
}
void Cell::add_entity(Entity *entity) {
  entities.push_back(entity);
  update_entities_shape(shape.getPosition());
}
void Cell::remove_entity(Entity *entity){
  auto element = std::find(entities.begin(),entities.end(),entity);
  if(element != entities.end())
    entities.erase(element);
  else std::cout << "no such entity on given cell" << std::endl;
}
Cell *Map::get_clicked_cell(const int x, const int y) {
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
Entity* Cell::get_entity_clicked(const int x, const int y){
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
  sf::Vector2f index = c->get_simulation_cell()->get_index();
  std::vector<Cell *> nearbycells = getnearbycells(index);
  for(size_t i = 0; i < nearbycells.size(); i++){
    if(pred(nearbycells[i]))
      nearbycells[i]->set_selected();
  }
}
void Map::select_accessible_cells(Champion *champ){
  setselectednearbycells(champ,[](Cell *c){return c->can_move_here();});
}
void Map::select_wardable_cells(Champion *champ){
  setselectednearbycells(champ,[](Cell *c){return c->can_ward_here();});
}
void Map::select_attackable_entities(Champion *c){
  setselectednearbycells(c,[](Cell *c){return c->can_attack_entity();});
}
void Cell::unselect(){
  selected = false;
}
void Cell::set_selected(){
  set_highlighted();
  selected = true;
}
void Map::move(Entity *entity, sf::Vector2f from, sf::Vector2f to){
  posindex from_index = toposindex(from); 
  posindex to_index = toposindex(to);
  cells[from_index.i][from_index.j]->remove_entity(entity);
  cells[to_index.i][to_index.j]->add_entity(entity);
  // update the vision
  update_vision();
  //entity->update_shape_pos(position);
}