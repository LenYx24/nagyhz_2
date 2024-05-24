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
  for(auto & entity : entities){
    entity->update_shape_pos(mappos);
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
  has_vision = true;
  sf::Color c = shape.getFillColor();
  if(!selected){
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
  for(auto & entity : entities){
    if(entity != nullptr && has_vision && entity->is_alive()) //
      entity->draw(w);
  }
}
bool Map::in_bounds(sf::Vector2f index){
  return in_bounds_row(static_cast<int>(index.x)) && in_bounds_col(static_cast<int>(index.y));
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
  vision_side = Side::NEUTRAL;
  position = pos;
  std::string map_filename = "resources/map.txt";
  std::ifstream file(map_filename);
  if (!file) {
    throw std::invalid_argument("error with opening "+map_filename);
  }
  // first set every cell to a nullptr
  for(size_t i = 0; i < size_x; i++){
    for(size_t j = 0; j < size_y; j++){
      cells[i][j] = nullptr;
    }
  }
  // load the map
  for (size_t i = 0; i < size_x; i++) {
    std::string line;
    std::getline(file, line);
    size_t line_index = 0;
    for (size_t j = 0; j < size_y; j++) {
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
        auto tower = new Tower;
        tower->set_cell(cells[i][j]);
        tower->set_side(Side::RED);
        cells[i][j]->add_entity(tower);
        break;
      }
      // the blue side turret
      case 'z': {
        cells[i][j] = new Ground{};
        auto tower = new Tower;
        tower->set_side(Side::BLUE);
        cells[i][j]->add_entity(tower);
        break;
      }
      case 'n': {
        cells[i][j] = new Ground{};
        auto nexus = new Nexus;
        nexus->set_side(Side::RED);
        nexuses.push_back(nexus);
        cells[i][j]->add_entity(nexus);
        break;
      }
      case 'm': {
        cells[i][j] = new Ground{};
        auto nexus = new Nexus;
        nexus->set_side(Side::BLUE);
        cells[i][j]->add_entity(nexus);
        break;
      }
      // red jungle camp
      case 'j': {
        cells[i][j] = new Ground{};
        cells[i][j]->add_entity(new Camp);
        break;
      }
      // baron
      case 'a': {
        cells[i][j] = new Ground{};
        Camp *baron = new Camp{300,30};
        baron->set_name("baron");
        baron->set_color(sf::Color{130,50,170});
        baron->set_effect(Effect{200, 200, true, 9});
        cells[i][j]->add_entity(baron);
        break;
      }
      // drake
      case 'd': {
        cells[i][j] = new Ground{};
        cells[i][j]->add_entity( new Drake);
        break;
      }
      // redbuff
      case 'r': {
        cells[i][j] = new Ground{};
        Camp *c = new Camp;
        c->set_name("red buff");
        c->set_color(sf::Color{250,50,50});
        c->set_effect(Effect{10, 0, true, 9});
        cells[i][j]->add_entity(c);
        break;
      }
      // bluebuff
      case 'c': {
        cells[i][j] = new Ground{};
        Camp *c = new Camp;
        c->set_name("blue buff");
        c->set_color(sf::Color{50,50,250});
        c->set_effect(Effect{0, 10,true,9});
        cells[i][j]->add_entity(c);
        break;
      }
      case 'x':{
        cells[i][j] = new SpawnArea{};
        break;
      }
      default: {
        cells[i][j] = nullptr;
        throw std::invalid_argument("err: wrong symbol: " + std::to_string(line[line_index]));
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
void Map::draw(sf::RenderWindow &window) {
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->draw(window);
      }
    }
  }
}
void Map::reset_cell_selections(){
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->unselect();
        cells[i][j]->reset_selection_color();
      }
    }
  }
}
void Map::reset_cell_vision(){
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if (cells[i][j] != nullptr) {
        cells[i][j]->reset_vision_color();
      }
    }
  }
}
void Map::disable_vision() {
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
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
void Map::de_spawn(Entity *entity){
  Cell *cell = entity->get_real_cell();
  sf::Vector2f pos = cell->get_index();
  posindex pindex = toposindex(pos);
  bool removed = cells[pindex.i][pindex.j]->remove_entity(entity);
  // this means the entity was not found at the given cell
  // then we look around to see if its on the map
  if(!removed){
    for(size_t i = 0; i < size_x; i++){
      for(size_t j = 0; j < size_y; j++){
        if(cells[i][j] != nullptr){
          bool did_remove = cells[i][j]->remove_entity(entity);
          // if we removed the entity from the map, then successfully return
          if(did_remove)return;
        }
      }
    }
  }
}
Map::~Map() {
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if(cells[i][j] != nullptr){
        delete cells[i][j];
        cells[i][j] = nullptr;
      }
    }
  }
}
Cell::~Cell(){
  std::cout << "starting to delete entities for current cell" << std::endl;
  for(auto & entity : entities){
    std::cout << "deleted entity name: " << entity->get_stats()[0] << std::endl;
    delete entity;
  }
  std::cout << "deleted entities for current cell" << std::endl;
}
bool Cell::should_update_vision_around(Side current_side){
  for(auto & entity : entities){
    if(entity->get_side() == current_side && entity->gives_vision() && entity->is_alive()){
      return true;
    }
  }
  return false;
}
void Cell::do_attack(Map *map){
  for(auto entity : entities){
    entity->attack(map);
  }
}
void Map::update(){
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if(cells[i][j] != nullptr){
        cells[i][j]->update(this);
      }
    }
  }
}
void Cell::update(Map *map){
  for(auto entity: entities){
    // update entity
    entity->respawn();
    entity->attack(map);
  }
}
void Map::do_attack(){
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if(!cells[i][j]){
        cells[i][j]->do_attack(this);
      }
    }
  }
}
sf::Uint8 Cell::has_vision_opacity = 255;
sf::Uint8 Cell::no_vision_opacity = 50;
sf::Uint8 Cell::selected_opacity = 100;
void Map::update_vision(){
  for(size_t i = 0; i < size_x; i++){
    for(size_t j = 0; j < size_y; j++){
      cells[i][j]->set_vision(false);
    }
  }
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
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
  for(auto & entity : entities){
    if(entity->get_side() != side_ && entity->is_alive())return entity;
  }
  return nullptr;
}
bool Map::check_game_end(){
  for(Entity *nexus: nexuses){
    if(!nexus->is_alive()){
      std::cout << "nexus just died, game ended!" << std::endl;
      return true;
    }
  }
  return false;
}
void Cell::set_vision(bool has_vision_){
  has_vision = has_vision_;
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
bool Cell::remove_entity(Entity *entity){
  auto element = std::find(entities.begin(),entities.end(),entity);
  if(element != entities.end()){
    entities.erase(element);
    return true;
  }
  return false;
}
Cell *Map::get_clicked_cell(const int x, const int y) {
  for (size_t i = 0; i < size_x; i++) {
    for (size_t j = 0; j < size_y; j++) {
      if (cells[i][j]->contains(x, y)) {
        return cells[i][j];
      }
    }
  }
  return nullptr;
}
bool Cell::contains(const int x, const int y) {
  return shape.getGlobalBounds().contains(static_cast<float>(x),static_cast<float>(y));
}
Entity* Cell::get_entity_clicked(const int x, const int y){
  // need to start iterating from the back, because of how the entities are drawn to the screen, 
  //the last one is drawn on top of the other ones
  for(int i = static_cast<int>(entities.size())-1; i >=0; i--){
    size_t index = static_cast<size_t>(i);
    if(entities[index]->clicked(x,y))return entities[index];
  }
  return nullptr;
}
std::vector<Cell*> Map::getnearbycells(sf::Vector2f pos, int distance){
  std::vector<Cell *> around;
  sf::Vector2 pos_ = {static_cast<int>(pos.x),static_cast<int>(pos.y)};
  for(int i = pos_.x-distance; i <= pos_.x+distance; i++){
    if(in_bounds_row(static_cast<int>(i))){
      for(int j = pos_.y-distance; j <= pos_.y+distance; j++){
        size_t index_i = static_cast<size_t>(i);
        size_t index_j = static_cast<size_t>(j);
        if(in_bounds_col(j) && cells[index_i][index_j] != nullptr){
          around.push_back(cells[index_i][index_j]);
        }
      }
    }
    
  }
  return around;
}
template<typename P>
void Map::set_selected_nearby_cells(Champion *champ, P pred){
  sf::Vector2f index = champ->get_simulation_cell()->get_index();
  std::vector<Cell *> nearbycells = getnearbycells(index);
  for(size_t i = 0; i < nearbycells.size(); i++){
    if(pred(nearbycells[i])){
      nearbycells[i]->set_selected();
    }
  }
}
void Map::select_accessible_cells(Champion *champ){
  set_selected_nearby_cells(champ, [](Cell *c) { return c->can_move_here(); });
}
void Map::select_wardable_cells(Champion *champ){
  set_selected_nearby_cells(champ, [](Cell *c) { return c->can_ward_here(); });
}
void Map::select_attackable_entities(Champion *champ){
  set_selected_nearby_cells(champ, [champ](Cell *c) { return c->can_attack_entity(champ->get_side()); });
}
bool Cell::can_attack_entity(Side enemy_side_)const{
  for(Entity *entity: entities){
    if(entity->get_side() != enemy_side_ && entity->is_alive())return true;
  }
  return false;
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
}