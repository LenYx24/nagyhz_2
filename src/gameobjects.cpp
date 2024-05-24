#include "../include/gameobjects.hpp"
#include "SFML/Graphics/Color.hpp"
#include <cstdlib>
#include <utility>

Entity::Entity(std::string name) : name(std::move(name)) {
  shape.setSize({15,15});
  shape.setFillColor(sf::Color::Cyan);
}

void Ward::do_move(){
  if(cooldown > 0){
    cooldown--;
  }
  else{
    alive = false;
  }
}
Champion::Champion(const std::string& name_, double damage_, double dmg_per_level_, double hp_, double hp_per_level_){
  name = name_;
  damage = damage_;
  dmg_per_level = dmg_per_level_;
  base_hp = hp_;
  current_hp = hp_;
  hp_per_level = hp_per_level_;
  icon.setCharacterSize(10);
  icon.setFillColor(sf::Color::White);
}
Ward::Ward():cooldown(9){
  set_color(sf::Color{186,48,133});
  alive=true;
}
Player::Player(std::vector<Champion*> champs):side(Side::BLUE),starter(false){
  this->champs = std::move(champs);
  gamemoveactive = false;
  minion_timer = 0;
  minion_timer_mark = 3;
  is_simulation = false;
  spawn_point = nullptr;
}
Item::Item(std::string name_, int gold_, double bonus_dmg_, double bonus_hp_){
  name = std::move(name_);
  gold_value = gold_;
  set_bonus_dmg(bonus_dmg_);
  set_bonus_hp(bonus_hp_);
}
void Entity::attack(Map *){}
void Player::spawn_minions(const std::shared_ptr<Map>& map){
  sf::Vector2f offset = {2,2};
  // because the map is drawn from top left corner, I need to subtract y if I want to move up
  sf::Vector2f sign = {1,-1};
  if(side == Side::RED)sign=-sign;
  std::vector<sf::Vector2f> points;
  std::vector<std::vector<sf::Vector2f>> directions;
  sf::Vector2f spawn_point_index = spawn_point->get_index();

  // top
  std::vector<sf::Vector2f> dir_top = {
      {0,sign.y*1},
      {sign.x*1,0}
  };
  // need to reverse the sequence, because on redside the minion first moves horizontally left, then vertically down
  // but on the blue side it first goes up then right (first vertically then horizontally)
  if(side == Side::RED)
    std::reverse(dir_top.begin(), dir_top.end());
  directions.push_back(dir_top);
  // mid
  std::vector<sf::Vector2f> dir_mid = {{sign.x*1,sign.y*1}};
  directions.push_back(dir_mid);
  // bot
  std::vector<sf::Vector2f> dir_bot = {{sign.x*1,0},{0,sign.y*1}};
  if(side == Side::RED)
    std::reverse(dir_bot.begin(), dir_bot.end());
  directions.push_back(dir_bot);
  float x=0,y=0;
  if(side ==Side::BLUE){
      x = spawn_point_index.x;
      y = spawn_point_index.y+offset.y*sign.y;
      points.emplace_back(x,y);
      x = spawn_point_index.x+offset.x*sign.x;
      y = spawn_point_index.y+offset.y*sign.y;
      points.emplace_back(x,y);
      x = spawn_point_index.x+offset.x*sign.x;
      y = spawn_point_index.y;
      points.emplace_back(x,y);
  }else{
    x = spawn_point_index.x+offset.x*sign.x;
    y = spawn_point_index.y;
    points.emplace_back(x, y);
    x = spawn_point_index.x+offset.x*sign.x;
    y = spawn_point_index.y+offset.y*sign.y;
    points.emplace_back(x,y);
    x = spawn_point_index.x;
    y = spawn_point_index.y+offset.y*sign.y;
    points.emplace_back(x,y);
  }

  // spawn
  for(size_t i = 0; i < points.size(); i++){
    MinionWave wave;
    wave.spawn(points[i],directions[i],map, side);
    minion_waves.push_back(wave);
  }
}
void Player::round_end(std::shared_ptr<Map> &map){
  minion_timer++;
  if(minion_timer == minion_timer_mark){
    spawn_minions(map);
    minion_timer = 0;
  }
  for(auto & champ : champs){
    champ->round_end(map);
  }
  for(auto wave: minion_waves){
    wave.round_end();
  }
}
void Champion::add_gamemove(GameMove *move){
  gamemoves.push_back(move);
  current_gamemove = gamemoves.back();
}
std::string Player::get_gamemoves_state()const{
  std::string full;
  for(auto champ: champs){
    full += champ->get_current_gamemove_state_info();
  }
  return full;
}
std::string Champion::get_current_gamemove_state_info()const{
  std::string info;
  std::vector<std::string> stats = get_stats();
  for(const auto& stat: stats){
    info+=stat + ":";
  }
  if(current_gamemove == nullptr){
    info+='\n';
    return info;
  }
  info += current_gamemove->get_state_info();
  return info;
}
void Champion::round_end(std::shared_ptr<Map> map){
  movepoints = 3;
  current_gamemove = nullptr;
  for(GameMove *move : gamemoves){
    delete move;
  }

  gamemoves.clear();
  simulation_points_counter = 1;
  // passive gold generation
  gold+=20;
  // passive hp regen
  int passive_hp_regen = 5;
  if(current_hp+passive_hp_regen <=get_max_hp()){
    current_hp+=passive_hp_regen;
  }

  // check if any wards expired and remove them
  for(auto iter = wards.begin(); iter != wards.end(); iter++){
    if(*iter != nullptr && !(*iter)->is_alive()){
      std::cout << "despawning ward" << std::endl;
      map->de_spawn(*iter,(*iter)->get_real_cell()->get_index());
      delete *iter;
      wards.erase(iter);
      *iter = nullptr;
      break;
    }
  }
}
void Entity::update_shape_pos(sf::Vector2f pos){
  shape.setPosition(pos + sf::Vector2f{5,5});
}
void Champion::update_shape_pos(sf::Vector2f pos){
  Entity::update_shape_pos(pos);
  icon.setPosition(shape.getPosition() + sf::Vector2f{2,2});
}
void Entity::draw(sf::RenderWindow &window){
  window.draw(shape);
}
void Champion::draw(sf::RenderWindow &window){
  window.draw(shape);
  window.draw(icon);
}
sf::Vector2f Minion::get_next_direction_pos_index(){
  if(cell == nullptr || !directions.empty()){
     return directions[0];
  }
  return sf::Vector2f{0, 0};
}

void Player::spawn_champs(const std::shared_ptr<Map> &map){
  for(auto & champ : champs){
    sf::Vector2f spawn_point_index = spawn_point->get_index();
    map->spawn(champ,spawn_point_index);
    champ->set_cell(map->getcell(spawn_point_index));
  }
}
void Player::do_moves(const std::shared_ptr<Map>& map){
  for(auto & champ : champs){
    champ->do_move(map);
  }
  for(auto & wave : minion_waves){
    wave.do_move(map);
  }
}
void Player::set_champ_icons(const std::string &icons){
  for(size_t i = 0; i < icons.length(); i++){
    champs[i]->set_icon(icons[i]);
  }
}
void Player::set_font(Resources::Holder &holder){
  for(auto & champ : champs){
    champ->set_font(holder);
  }
}
void Champion::set_font(Resources::Holder &h){
  icon.setFont(h.get(Resources::Type::FONT));
}
bool Entity::clicked(const int x, const int y){
  return shape.getGlobalBounds().contains(static_cast<float>(x),static_cast<float>(y));
}
std::string Entity::to_ui_int_format(double num){
  return std::to_string(static_cast<int>(num));
}
std::vector<std::string> Entity::get_stats()const{
  std::vector<std::string> stats;
  stats.push_back("name: "+name);
  stats.push_back("current_hp: "+to_ui_int_format(current_hp));
  stats.push_back("max_hp: "+to_ui_int_format(get_max_hp()));
  stats.push_back("damage: "+to_ui_int_format(damage));
  return stats;
}
std::vector<std::string> Ward::get_stats()const{
  std::vector<std::string> stats = Entity::get_stats();
  stats.emplace_back("cooldown: "+ to_ui_int_format(cooldown));
  return stats;
}
std::vector<std::string> Champion::get_stats()const{
  std::vector<std::string> stats = Entity::get_stats();
  stats.push_back("movepoints: "+std::to_string(movepoints));
  stats.push_back("level: "+std::to_string(level));
  stats.push_back("gold: "+std::to_string(gold));
  stats.push_back("experience: "+std::to_string(xp));
  stats.emplace_back("items: " + std::to_string(items.size()));
  for(auto & item : items){
    stats.push_back(item->get_name());
  }
  stats.emplace_back("buffs: " + std::to_string(items.size()));
  for(auto & buff : buffs){
    std::string b_hp = std::to_string(buff.get_bonus_hp());
    std::string b_dmg = std::to_string(buff.get_bonus_dmg());
    std::string buff_stat = "buff: bonus hp: ";
    buff_stat += b_hp;
    buff_stat += " bonus dmg: ";
    buff_stat += b_dmg;
    stats.push_back(buff_stat);
  }
  return stats;
}
Minion::Minion(Side side_, std::vector<sf::Vector2f> directions_, Cell *spawn_point){
  directions = std::move(directions_);
  side = side_;
  cell = spawn_point;
  set_color(sf::Color{100,165,90});
  set_name("minion");
  base_hp = 40;
  current_hp = base_hp;
  damage = 5;
  shape.setSize({10,10});
}
Tower::Tower(){
  name = "tower";
  base_hp = 100;
  current_hp = base_hp;
  damage = 30;
  xp_given = 40;
  shape.setFillColor(sf::Color{230,10,90});
}
Camp::Camp(){
  name = "camp";
  base_hp = 100;
  current_hp = base_hp;
  damage = 15;
  set_xp_given(30);
  set_side(Side::NEUTRAL);
  shape.setFillColor(sf::Color{130,100,230});
}
Nexus::Nexus(){
  name = "nexus";
  base_hp = 300;
  current_hp = base_hp;
  damage = 0;
  shape.setFillColor(sf::Color{100,50,88});
}
bool Player::is_his_champ(Champion *c){
  if(c == nullptr)
      return false;
  for(auto & champ : champs){
    if(champ->get_name() == c->get_name())
        return true;
  }
  return false;
}
void Champion::remove_last_gamemove(){
  if(!gamemoves.empty()){
    if(gamemoves.back()->is_complete()){
      movepoints += gamemoves.back()->get_movepoints();
    }
    delete gamemoves.back();
    gamemoves.pop_back();
    if(!gamemoves.empty())
      current_gamemove = gamemoves.back();
    else
      current_gamemove = nullptr;
  }
}

Champion *Player::get_selected_champs(sf::Vector2f index){
  for(int i = static_cast<int>(champs.size())-1; i >=0; i--){
    auto current_index = static_cast<size_t>(i);
    if(champs[current_index]->get_simulation_cell()->get_index() == index)
      return champs[current_index];
  }
  return nullptr;
}
bool Player::is_gamemove_active() const{
  return gamemoveactive;
}

Champion::~Champion(){
  for(auto & gamemove : gamemoves){
    delete gamemove;
  }
}

void Champion::finish_gamemove(Cell *cell){
  if(current_gamemove != nullptr){
    current_gamemove->finish(cell);
    movepoints-= current_gamemove->get_movepoints();
  }
}
void Entity::check_death(){
  if(current_hp <= 0){
    respawn_counter = respawn_timer;
    alive = false;
  }
}
void Player::set_side(Side side_){
  side = side_;
  for(Champion *champ:champs){
    champ->set_side(side_);
  }
}
 void Entity::remove_hp(double dmg){
   current_hp-=dmg;
   check_death();
 }
void Tower::attack(Map *map){
  std::cout << "attack" << std::endl;
  if(!is_alive())return;
  // checks if there are nearby enemies, and attacks them
  std::vector<Cell *> nearby_cells = map->getnearbycells(cell->get_index());
  // first check if there are focusable entities to attack
  bool try_attack_focusables = false;
  for(size_t j = 0; j < 2; j++){
    for(auto & nearby_cell : nearby_cells){
      Entity *target = nearby_cell->get_attackable_entity(side);
      if(target != nullptr && try_attack_focusables && target->should_focus()){
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
      e.set_bonus_dmg(10);
      setEffect(e);
      break;
    }
    case 1:{
      e.set_bonus_hp(10);
      setEffect(e);
      break;
    }
    default:{
      break;
    }
  }
}
void Champion::place_ward(const std::shared_ptr<Map>& map, Cell *c){
  if(wards.size() < wards_max){
    Ward *ward = new Ward;
    ward->set_side(side);
    ward->set_cell(c);
    map->spawn(ward, c->get_index());
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
    map->de_spawn(champ, champ->get_real_cell()->get_index());
  }
}
Player::~Player(){
  for(Champion *champ: champs){
    delete champ;
  }
}
Champion::Champion(){
  icon.setCharacterSize(10);
  icon.setFillColor(sf::Color::White);
}
void Champion::add_item(Item *item){
  if(items.size() < 6 && item->get_gold_value() <= gold){
    items.push_back(item);
    gold -= item->get_gold_value();
  }
}
sf::Vector2f Champion::gamemove_index(size_t offset)const{
  if(!current_gamemove)
    throw std::runtime_error("current game move is a nullptr");

  int last_index = static_cast<int>(gamemoves.size()) - 1;
  for(int i = last_index - static_cast<int>(offset); i >= 0; i--){
    auto index = static_cast<size_t>(i);
    if(gamemoves[index]->position_cell() != nullptr){
      return gamemoves[index]->position_cell()->get_index();
    }
  }
  if(cell == nullptr)
    return {-1,-1};
  return cell->get_index();
}
sf::Vector2f Champion::last_gamemove_index()const{
  return gamemove_index(0);
}
sf::Vector2f Champion::current_gamemove_index()const{
  return gamemove_index(1);
}
Cell *Champion::get_simulation_cell(){
  int last_index = static_cast<int>(gamemoves.size()) - 1;
  for(int i = last_index; i >= 0; i--){
    auto index = static_cast<size_t>(i);
    if(gamemoves[index]->position_cell() != nullptr
        && gamemoves[index]->changes_pos()){
      return gamemoves[index]->position_cell();
    }
  }
  return cell;
}
void Champion::move(std::shared_ptr<Map> &map){
  if(!current_gamemove)return;
  if(current_gamemove->changes_pos()){
    map->move(this, current_gamemove_index(), last_gamemove_index());
  }
}
bool Player::check_round_end(){
  for(auto & champ : champs){
    if(champ->getmovepoints() !=0)return false;
  }
  return true;
}
void MinionWave::spawn(
    sf::Vector2f startpoint,
    const std::vector<sf::Vector2f>& directions_,
    const std::shared_ptr<Map>& map, Side side_){
    directions = directions_;
    for(size_t i = 0; i < minion_wave_size; i++){
      auto minion = new Minion{side_,directions_,map->getcell(startpoint)};
      minions.push_back(minion);
      map->spawn(minions.back(),startpoint);
    }
}
bool Champion::is_gamemove_complete() const {
  if(gamemoves.empty())return true;
  return current_gamemove->is_complete();
}
void MinionWave::round_end(){
  for(auto &minion: minions){
    minion->check_death();
  }
}
void Champion::set_spawn_point(Cell *spawn_point_){
  if(spawn_point_ != nullptr)spawn_point = spawn_point_;
}
void Player::set_spawn_point(Cell *spawn_point_){
  if(spawn_point_ != nullptr){
    spawn_point = spawn_point_;
    for(Champion *champ: champs){
      champ->set_spawn_point(spawn_point);
    }
  }
}
double Champion::get_max_hp()const {
  double hp = base_hp;
  for(auto item: items){
    hp+=item->get_bonus_hp();
  }
  return hp;
}
void Champion::do_move(std::shared_ptr<Map> map){
  if(!is_alive()){
    respawn_counter--;
    if(respawn_counter == 0){
      refill_hp();
      if(spawn_point!=nullptr){
        cell = spawn_point;
      }
      alive = true;
    }
    return;
  }
  if(!gamemoves.empty()){
    GameMove *first = *gamemoves.begin();
    if(first->get_movepoints() == simulation_points_counter && first->is_complete()){
      first->do_move(this,std::move(map));
      delete first;
      current_gamemove = nullptr;
      gamemoves.erase(gamemoves.begin());
      simulation_points_counter = 1;
    }else{
      simulation_points_counter++;
    }
  }
  for(auto & ward : wards){
    ward->do_move();
  }
}
void MinionWave::do_move(const std::shared_ptr<Map> &map){
  for(auto minion: minions){
    minion->do_move(map);
  }
}
void Minion::do_move(const std::shared_ptr<Map> &map){
  if(!cell)return;
  sf::Vector2f next_cell_dir = get_next_direction_pos_index();
  sf::Vector2f next_cell_index = cell->get_index()+next_cell_dir;
  if(!map->in_bounds(next_cell_index)){
    if(directions.empty()){
      Entity *enemy = cell->get_attackable_entity(side);
      if(enemy != nullptr){
        enemy->remove_hp(damage);
        remove_hp(enemy->get_total_dmg());
      }
    }
    directions.erase(directions.begin());
    next_cell_index = cell->get_index()+next_cell_dir;
  }
  Cell *next_cell = map->getcell(next_cell_index);
  if(!next_cell)return;
  // if there are enemies on the next cell the minion wants to go to, then it attacks the enemy
  Entity *enemy = next_cell->get_attackable_entity(side);
  if(enemy != nullptr){
    std::cout << "minion found an enemy" << std::endl;
    enemy->remove_hp(damage);
    remove_hp(enemy->get_total_dmg());
  }
  // if there are not then it just moves to the next cell
  else{
    map->move(this,cell->get_index(),next_cell->get_index());
    cell = next_cell;
  }
}
void Player::set_simulation(bool sim){
  for(auto & champ : champs){
    champ->set_simulation(sim);
  }
  is_simulation = sim;
}
void Player::update_champ_positions(const std::shared_ptr<Map>& map){
  for(auto & champ : champs){
    if(is_simulation){
      map->move(champ,
                champ->get_real_cell()->get_index(),
                champ->get_simulation_cell()->get_index());
    }else{
      map->move(champ,
                champ->get_simulation_cell()->get_index(),
                champ->get_real_cell()->get_index());
    }
  }
}

void Champion::fight(Entity *other){
  double total_dmg = get_total_dmg();
  double other_total_dmg = other->get_total_dmg();
  // if one of them can kill the other
  if(other->can_fight_back()
      &&
      (total_dmg >= other->get_current_hp() || other_total_dmg >= current_hp)){
    std::cout << "champion fight" << std::endl;
    double chance = ((total_dmg+other_total_dmg)/total_dmg + (current_hp+other->get_current_hp())/current_hp) /2;
    int ran = rand();
    std::cout << "change for the fight: " << chance << std::endl;
    std::cout << "random number: " << ran << std::endl;
    // the champ won
    if(chance <= ran){
      other->remove_hp(damage);
      killed_other(other);
    }
    else{ // the other entity won
      remove_hp(other_total_dmg);
      other->killed_other(this);
    }
  }
  else{
    remove_hp(other_total_dmg);
    other->remove_hp(total_dmg);
  }
}
void Entity::killed_other(Entity *other){
  other->remove_hp(5);
}
void Champion::killed_other(Entity *other){
  if (other->gives_creep_score())
    cs++;
  add_xp(other->get_xp_given());
  gold += other->get_gold_given();
}

void Champion::add_xp(int xp_){
  xp+=xp_;
  if(xp >= xp_cutoff && level < max_level){
    // then level up
    xp = 0;
    damage+=dmg_per_level;
    base_hp +=hp_per_level;
    level++;
    xp_cutoff += level_xp_increase;
  }
}