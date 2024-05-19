#include "../include/gameobjects.hpp"
#include "SFML/Graphics/Color.hpp"
#include <cstdlib>

Entity::Entity(std::string name) : name(name) {
  shape.setSize({15,15});
  shape.setFillColor(sf::Color::Cyan);
  damage = 10;
  hp = 10;
  cell = nullptr;
  max_hp = 20;
  respawn_counter = 0;
  respawn_timer = 5;
  xp_given = 10;
}


std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}
void Champion::read_from_string(std::string &line, const char delimiter) {
  std::vector<std::string> tokens = splitString(line, delimiter);
  if(tokens.size() < 5){
    throw "wrong file format";
  }
  name = tokens[0];
  damage = std::stod(tokens[1]);
  dmg_per_level = std::stoi(tokens[2]);
  hp = std::stod(tokens[3]);
  hp_per_level = std::stoi(tokens[4]);
  cell = nullptr;
}
void Item::read_from_string(std::string &line, const char delimiter) {
  // Todo: read item datas
  std::vector<std::string> tokens = splitString(line, delimiter);
  if(tokens.size() < 4){
    throw "wrong file format";
  }
  name = tokens[0];
  gold_value = std::stoi(tokens[1]);
  setbonusdmg(std::stod(tokens[2]));
  setbonushp(std::stod(tokens[3]));
}
Player::Player(std::vector<Champion*> champs):side(Side::BLUE),starter(false){
  this->champs = std::move(champs);
  gamemoveactive = false;
  minion_timer = 0;
  minion_timer_mark = 3;
  is_simulation = false;
}
void Player::spawn_minions(const std::shared_ptr<Map>& map){
  float offset = 2;
  float sign = 1;
  if(side == Side::RED)sign=-sign;
  std::vector<sf::Vector2f> points;
  std::vector<std::vector<sf::Vector2f>> directions;

  // top
  std::vector<sf::Vector2f> dir_top = {{0,sign*1},{sign*1,0}};
  points.emplace_back(spawnpoint.x+offset*sign,spawnpoint.y);
  // need to reverse the sequence, because on redside the minion first moves horizontally left, then vertically down
  // but on the blue side it first goes up then right (first vertically then horizontally)
  if(side == Side::RED)std::reverse(dir_top.begin(), dir_top.end());
  directions.push_back(dir_top);
  // mid
  std::vector<sf::Vector2f> dir_mid = {{sign*1,sign*1}};
  directions.push_back(dir_mid);
  points.emplace_back(spawnpoint.x+offset*sign,spawnpoint.y-offset*sign);
  // bot
  std::vector<sf::Vector2f> dir_bot = {{sign*1,0},{0,sign*1}};
  if(side == Side::RED)std::reverse(dir_top.begin(), dir_top.end());
  directions.push_back(dir_bot);
  points.emplace_back(spawnpoint.x,spawnpoint.y-offset*sign);

  // spawn
  for(size_t i = 0; i < points.size(); i++){
    MinionWave *wave = new MinionWave;
    wave->spawn(points[i],directions[i],map, side);
    minion_waves.push_back(wave);
  }
}
void Player::round_end(std::shared_ptr<Map> map){
  minion_timer++;
  if(minion_timer == minion_timer_mark){
    spawn_minions(map);
    minion_timer = 0;
  }
  for(auto & champ : champs){
    champ->round_end();
  }
}
void Champion::round_end(){
  movepoints = 3;
  //gamemoves.clear();
  //current_gamemove = nullptr;
  //simulation_points_counter = 1;
  // passive gold generation
  gold+=2;
}
void Champion::update_total_dmg(){
  double total_dmg_ = get_base_dmg();
  for(auto &item: items){
    total_dmg_+=item->getbonusdmg();
  }
  total_damage = total_dmg_;
}
void Champion::update_total_hp(){
  double total_hp_ = get_base_hp();
  for(auto &item: items){
    total_hp_+=item->getbonushp();
  }
  total_hp = total_hp_;
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
Minion::Minion(){
  set_color(sf::Color{100,165,90});
  this->set_name("minion");
  this->hp = 30;
  this->damage = 30;
  this->shape.setSize({10,10});
}
void Player::spawn_champs(const std::shared_ptr<Map> &map){
  for(auto & champ : champs){
    map->spawn(champ,spawnpoint);
    champ->set_cell(map->getcell(spawnpoint));
  }
}
void Player::domoves(std::shared_ptr<Map> map){
  for(auto & champ : champs){
    champ->do_move(map);
  }
}
void Player::setchampicons(const std::string &icons){
  for(size_t i = 0; i < icons.length(); i++){
    champs[i]->set_icon(icons[i]);
  }
}
void Player::setfont(Resources::Holder &h){
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->set_font(h);
  }
}
void Champion::set_font(Resources::Holder &h){
  icon.setFont(h.get(Resources::Type::FONT));
}
bool Entity::clicked(const int x, const int y){
  return shape.getGlobalBounds().contains(static_cast<float>(x),static_cast<float>(y));
}
// bool Tower::clicked(const int x, const int y){
//   return true;
// }
std::string Entity::to_ui_int_format(double num){
  return std::to_string(static_cast<int>(num));
}
std::vector<std::string> Entity::get_stats(){
  std::vector<std::string> stats;
  stats.push_back("name: "+name);
  stats.push_back("hp: "+to_ui_int_format(hp));
  stats.push_back("max_hp: "+to_ui_int_format(max_hp));
  stats.push_back("dmg: "+to_ui_int_format(damage));
  return stats;
}
std::vector<std::string> Champion::get_stats(){
  std::vector<std::string> stats = Entity::get_stats();
  stats.push_back("movepoints: "+std::to_string(movepoints));
  stats.push_back("items: ");
  for(auto & item : items){
    stats.push_back(item->getname());
  }
  return stats;
}
Tower::Tower(){
  name = "tower";
  max_hp = 100;
  hp = 100;
  damage = 10;
  xp_given = 40;
  shape.setFillColor(sf::Color{230,10,90});
}
Camp::Camp(){
  name = "camp";
  max_hp = 100;
  hp = 100;
  damage = 15;
  set_xp_given(30);
  shape.setFillColor(sf::Color{130,100,230});
}
Nexus::Nexus(){
  name = "nexus";
  max_hp = 300;
  hp = 300;
  damage = 0;
  shape.setFillColor(sf::Color{100,50,88});
}
bool Player::ishischamp(Champion *c){
  if(c == nullptr)return false;
  for(size_t i = 0; i < champs.size(); i++){
    if(champs[i]->get_name() == c->get_name())return true;
  }
  return false;
}
Champion *Player::getselectedchamp(sf::Vector2f index){
  for(int i = static_cast<int>(champs.size())-1; i >=0; i--){
    size_t current_index = static_cast<size_t>(i);
    if(champs[current_index]->get_simulation_cell()->getindex() == index)return champs[current_index];
  }
  return nullptr;
}
bool Player::is_gamemove_active() const{
  return gamemoveactive;
}
void Player::setgamemoveactive(bool b){
  gamemoveactive = b;
}

Champion::~Champion(){
  for(auto & gamemove : gamemoves){
    delete gamemove;
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
void Player::set_side(Side side_){
  side = side_;
  for(Champion *champ:champs){
    champ->set_side(side_);
  }
}
void Tower::attack(std::shared_ptr<Map> &map){
  // checks if there are nearby enemies, and attacks them
  std::vector<Cell *> nearby_cells = map->getnearbycells(cell->getindex());
  // first check if there are focusable entities to attack
  bool try_attack_focusables = false;
  for(size_t j = 0; j < 2; j++){
    for(size_t i = 0; i < nearby_cells.size(); i++){
      Entity *target = nearby_cells[i]->get_attackable_entity(side);
      if(target != nullptr && try_attack_focusables && target->should_focus()){
        // do fight
        target->remove_hp(damage);
        remove_hp(target->get_total_dmg());
        break;
      }
    }
    // if not, then try to attack anything that is enemy
    try_attack_focusables = true;
  }

}
Drake::Drake(){
  set_name("drake");
  shape.setFillColor(sf::Color{235,200,60});
  decide_which_type();
}
void Drake::decide_which_type(){
  int type = rand() % 2 +1;
  Effect e;
  switch(type){
    case 0:{
      e.setbonusdmg(10);
      setEffect(e);
      break;
    }
    case 1:{
      e.setbonushp(10);
      setEffect(e);
      break;
    }
  }
}
void Champion::place_ward(std::shared_ptr<Map> map, Cell *c){
  if(wards.size() < wards_max){
    Ward *ward = new Ward;
    ward->set_cell(c);
    map->spawn(ward, c->getindex());
    wards.push_back(ward);
  }
}
void Champion::clear_gamemoves(){
  for(GameMove *gamemove: gamemoves){
    delete gamemove;
  }
  current_gamemove = nullptr;
}
void Player::clear_gamemoves(){
  for(Champion *champ: champs){
    champ->clear_gamemoves();
  }
}
void Player::despawn_champs(std::shared_ptr<Map> &map){
  for(Champion *champ: champs){
    map->despawn(champ, champ->get_real_cell()->getindex());
  }
}
Player::~Player(){
  for(Champion *champ: champs){
    delete champ;
  }
  for(MinionWave *wave: minion_waves){
    delete wave;
  }
}
Champion::Champion(){
  current_gamemove = nullptr;
  movepoints = 3;
  icon.setCharacterSize(10);
  icon.setFillColor(sf::Color::White);
}
void Champion::add_item(Item *item){
  if(items.size() < 6){
    items.push_back(item);
  }
  update_total_dmg();
  update_total_hp();
}
sf::Vector2f Champion::gamemove_index(size_t offset)const{
  if(!current_gamemove)throw "current game move is a nullptr";
  int last_index = static_cast<int>(gamemoves.size()) - 1;
  for(int i = last_index - static_cast<int>(offset); i >= 0; i--){
    size_t index = static_cast<size_t>(i);
    if(gamemoves[index]->position_cell() != nullptr){
      return gamemoves[index]->position_cell()->getindex();
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
void MinionWave::spawn(sf::Vector2f startpoint,std::vector<sf::Vector2f> directions, std::shared_ptr<Map> map, Side side_){
  this->directions = directions;
  for(size_t i = 0; i < minion_wave_size; i++){
    Minion *minion = new Minion;
    minion->set_side(side_);
    minions.push_back(minion);
    map->spawn(minion,startpoint);
  }
}
void MinionWave::round_end(){
  for(auto &minion: minions){
    minion->check_death();
  }
}
void Champion::do_move(std::shared_ptr<Map> map){
  if(!gamemoves.empty()){
    if(gamemoves[0]->get_movepoints() == simulation_points_counter){
      gamemoves[0]->do_move(this,map);
      delete gamemoves[0];
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
void Champion::fight(Entity *other){
  if(!isAlive() || !other->isAlive())return;
  // if the champ is in kill range
  double total_dmg = get_total_dmg();
  double total_hp = get_total_hp();
  double other_total_dmg = other->get_total_dmg();
  double other_total_hp = other->get_total_hp();
  // if one of them can kill the other
  if(total_dmg >= other_total_hp || other_total_dmg >= total_hp){
    if(other->can_fight_back()){
      double chance = ((total_dmg+other_total_dmg)/total_dmg + (total_hp+other_total_hp)/total_hp) /2;
      double ran = rand();
      std::cout << "change for the fight: " << chance << std::endl;
      std::cout << "random number: " << ran << std::endl;
      // the champ won
      if(chance <= ran){
        other->remove_hp(damage);
        if(!other->isAlive()) {
          if (other->gives_creep_score())
            cs++;
          add_xp(other->get_xp_given());
          gold += other->get_gold_given();
        }
      } 
      else{ // the other entity won
        remove_hp(other_total_dmg);
        if(!isAlive()){
          // Todo: give the enemy the xp and gold it got
        }
      }
    }
  }
  else{
    remove_hp(other_total_dmg);
    other->remove_hp(total_dmg);
  }
  // Todo: check if enemy is in execute range, then calculate chance of escape for them
}

void Champion::add_xp(int xp_){
  xp+=xp_;
  if(xp >= xp_cutoff && level < max_level){
    // then level up
    xp = 0;
    damage+=dmg_per_level;
    hp+=hp_per_level;
    level++;
    xp_cutoff += level_xp_increase;
    update_total_dmg();
    update_total_hp();
  }
}