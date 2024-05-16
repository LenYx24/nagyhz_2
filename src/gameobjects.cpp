#include "../include/gameobjects.hpp"

Entity::Entity(std::string name) : name(name) {
  shape.setSize({15,15});
  shape.setFillColor(sf::Color::Cyan);
}

void Champion::spawn() {
  // spawns the champion
}
void Champion::die() {
  // champions dies
}

void Champion::update_vision() {}

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
void Champion::readfromstring(std::string &line, const char delimiter) {
  // Todo: read champ data
  std::vector<std::string> tokens = splitString(line, delimiter);
  if(tokens.size() < 5){
    throw "wrong file format";
  }
  name = tokens[0];
  damage = std::stod(tokens[1]);
  dmg_per_level = std::stod(tokens[2]);
  hp =std::stod(tokens[3]);
  hp_per_level = std::stod(tokens[4]);
  cell = nullptr;
}
void Item::readfromstring(std::string &line, const char delimiter) {
  // Todo: read item datas
  std::vector<std::string> tokens = splitString(line, delimiter);
  if(tokens.size() < 4){
    throw "wrong file format";
  }
  name = tokens[0];
  gold_value = std::stod(tokens[1]);
  setbonusdmg(std::stod(tokens[2]));
  setbonushp(std::stod(tokens[3]));
}
Player::Player(std::vector<Champion*> champs):starter(false){
  this->champs = champs;
  gamemoveactive = false;
  minion_timer = 0;
  minion_timer_mark = 3;
  is_simulation = false;
}
void Player::spawn_minions(std::shared_ptr<Map> map){
  int offset = 2;
  if(side == Side::RED)offset=-offset;
  std::vector<sf::Vector2f> points;
  points.push_back({spawnpoint.x+offset,spawnpoint.y});
  points.push_back({spawnpoint.x+offset,spawnpoint.y+offset});
  points.push_back({spawnpoint.x,spawnpoint.y+offset});
  for(size_t i = 0; i < points.size(); i++){
    minion_waves[i]->spawn(points[i],map);
  }
}
void Player::round_end(std::shared_ptr<Map> map){
  minion_timer++;
  if(minion_timer == minion_timer_mark){
    spawn_minions(map);
  }
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->round_end();
  }
}
void Champion::round_end(){
  this->movepoints = 3;
}
void Entity::update_shape_pos(sf::Vector2f pos){
  shape.setPosition(pos + sf::Vector2f{5,5});
}
void Champion::update_shape_pos(sf::Vector2f pos){
  Entity::update_shape_pos(pos);
  icon.setPosition(shape.getPosition() + sf::Vector2f{2,2});
}
void Entity::draw(sf::RenderWindow &w){
  w.draw(shape);
}
void Champion::draw(sf::RenderWindow &window){
  window.draw(shape);
  window.draw(icon);
}
void Champion::setname(std::string name){this->name=name;}
void Player::spawnchamps(const std::shared_ptr<Map> map){
  for(size_t i = 0; i < champs.size(); i++){
    map->spawn(champs[i],spawnpoint);
    champs[i]->setcell(map->getcell(spawnpoint));
  }
}
void Player::domoves(std::shared_ptr<Map> map){
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->do_move(map);
  }
}
void Player::setchampicons(const std::string &icons){
  for(size_t i = 0; i < icons.length(); i++){
    champs[i]->seticon(icons[i]);
  }
}
void Player::setfont(Resources::Holder &h){
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->setfont(h);
  }
}
void Champion::setfont(Resources::Holder &h){
  icon.setFont(h.get(Resources::Type::FONT));
}
bool Entity::clicked(const int x, const int y){
  return shape.getGlobalBounds().contains(x,y);
}
// bool Tower::clicked(const int x, const int y){
//   return true;
// }
std::string Entity::to_ui_int_format(double num){
  return std::to_string(static_cast<int>(num));
}
std::vector<std::string> Entity::getstats(){
  std::vector<std::string> stats;
  stats.push_back("name: "+name);
  stats.push_back("hp: "+to_ui_int_format(hp));
  stats.push_back("dmg: "+to_ui_int_format(damage));
  return stats;
}
std::vector<std::string> Champion::getstats(){
  std::vector<std::string> stats = Entity::getstats();
  stats.push_back("movepoints: "+std::to_string(movepoints));
  return stats;
}
Tower::Tower(){
  name = "tower";
  maxhp = 300;
  hp = 300;
  damage = 30;
  shape.setFillColor(sf::Color{230,10,90});
}
Camp::Camp(){
  name = "camp";
  maxhp = 200;
  hp = 200;
  damage = 50;
  shape.setFillColor(sf::Color{130,100,230});
}
Nexus::Nexus(){
  name = "nexus";
  maxhp = 500;
  hp = 500;
  damage = 0;
  shape.setFillColor(sf::Color{100,50,88});
}
bool Player::ishischamp(Champion *c){
  if(c == nullptr)return false;
  for(size_t i = 0; i < champs.size(); i++){
    if(champs[i]->getname() == c->getname())return true;
  }
  return false;
}
Champion *Player::getselectedchamp(sf::Vector2f index){
  for(int i = static_cast<int>(champs.size())-1; i >=0; i--){
    if(champs[i]->get_simulation_cell()->getindex() == index)return champs[i];
  }
  return nullptr;
}
bool Player::is_gamemove_active(){
  return gamemoveactive;
}
void Player::setgamemoveactive(bool b){
  gamemoveactive = b;
}

Champion::~Champion(){
  for(size_t i = 0; i < gamemoves.size(); i++){
    delete gamemoves[i];
  }
}

void Player::draw_champs(sf::RenderWindow &window){
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->draw(window);
  }
}

void Champion::finish_gamemove(Cell *cell){
  if(current_gamemove != nullptr){
    current_gamemove->finish(cell);
    movepoints-= current_gamemove->get_movepoints();
  }
}
Champion::Champion(){
  current_gamemove = nullptr;
  movepoints = 3;
  icon.setCharacterSize(10);
  icon.setFillColor(sf::Color::White);
  simulation_points_counter = 1;
  simulation = false;
}
void Champion::add_item(Item *item){
  if(items.size() < 6){
    items.push_back(item);
  }
}
sf::Vector2f Champion::gamemove_index(size_t offset)const{
  if(!current_gamemove)throw "current game move is a nullptr";
  int lastindex = static_cast<int>(gamemoves.size()) - 1;
  for(int i = lastindex - offset; i >= 0; i--){
    if(gamemoves[i]->position_cell() != nullptr){
      return gamemoves[i]->position_cell()->getindex();
    }
  }
  return cell->getindex();
}
sf::Vector2f Champion::last_gamemove_index()const{
  return gamemove_index(0);
}
sf::Vector2f Champion::current_gamemove_index()const{
  return gamemove_index(1);
}
Cell *Champion::get_simulation_cell(){
  int lastindex = static_cast<int>(gamemoves.size()) - 1;
  for(int i = lastindex; i >= 0; i--){
    if(gamemoves[i]->position_cell() != nullptr){
      return gamemoves[i]->position_cell();
    }
  }
  return cell;
}
bool Player::check_round_end(){
  for(size_t i = 0; i < champs.size(); i++){
    if(champs[i]->getmovepoints() !=0)return false;
  }
  return true;
}
void MinionWave::spawn(sf::Vector2f startpoint, std::shared_ptr<Map> map){
  for(size_t i = 0; i < minion_wave_size; i++){
    Minion *minion = new Minion;
    minions.push_back(minion);
    map->spawn(minion,startpoint);
  }
}
void Champion::do_move(std::shared_ptr<Map> map){
  if(gamemoves.size() > 0){
    if(gamemoves[0]->get_movepoints() == simulation_points_counter){
      gamemoves[0]->do_move(this,map);
      gamemoves.erase(gamemoves.begin());
      simulation_points_counter = 1;
    }else{
      simulation_points_counter++;
    }
  }
}

void Player::set_simulation(bool sim){
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->set_simulation(sim);
  }
  is_simulation = sim;
}
void Player::update_champ_positions(std::shared_ptr<Map> map){
  for(size_t i = 0; i < champs.size(); i++){
    if(is_simulation){
      map->move(champs[i], champs[i]->get_real_cell()->getindex(), champs[i]->get_simulation_cell()->getindex());
    }else{
      map->move(champs[i], champs[i]->get_simulation_cell()->getindex(), champs[i]->get_real_cell()->getindex());
    }
  }
}