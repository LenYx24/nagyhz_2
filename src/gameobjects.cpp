#include "../include/gameobjects.hpp"
#include "SFML/Graphics/Color.hpp"
#include <cstdlib>
#include <utility>

Entity::Entity(std::string name) : name(std::move(name)) {
  shape.setSize({15,15});
  shape.setFillColor(sf::Color::Cyan);
}

void Ward::do_move(){
  cooldown--;
  if(cooldown == 0){
    // basically kill the ward, by reducing its hp
    alive = false;
  }
}
Champion::Champion(const std::string& name_, double damage_, double dmg_per_level_, double hp_, double max_hp_, double hp_per_level_){
  name = name_;
  damage = damage_;
  dmg_per_level = dmg_per_level_;
  hp = hp_;
  hp_per_level = hp_per_level_;
  max_hp = max_hp_;
  icon.setCharacterSize(10);
  icon.setFillColor(sf::Color::White);
}
Player::Player(std::vector<Champion*> champs):side(Side::BLUE),starter(false){
  this->champs = std::move(champs);
  gamemoveactive = false;
  minion_timer = 0;
  minion_timer_mark = 3;
  is_simulation = false;
}
Item::Item(std::string name_, int gold_, double bonus_dmg_, double bonus_hp_){
  name = std::move(name_);
  gold_value = gold_;
  set_bonus_dmg(bonus_dmg_);
  set_bonus_hp(bonus_hp_);
}

void Player::spawn_minions(const std::shared_ptr<Map>& map){
  float offset = 2;
  float sign = 1;
  if(side == Side::RED)sign=-sign;
  std::vector<sf::Vector2f> points;
  std::vector<std::vector<sf::Vector2f>> directions;

  // top
  std::vector<sf::Vector2f> dir_top = {{0,sign*1},{sign*1,0}};
  sf::Vector2f spawn_point_index = spawn_point->get_index();
  points.emplace_back(spawn_point_index.x+offset*sign,spawn_point_index.y);
  // need to reverse the sequence, because on redside the minion first moves horizontally left, then vertically down
  // but on the blue side it first goes up then right (first vertically then horizontally)
  if(side == Side::RED)std::reverse(dir_top.begin(), dir_top.end());
  directions.push_back(dir_top);
  // mid
  std::vector<sf::Vector2f> dir_mid = {{sign*1,sign*1}};
  directions.push_back(dir_mid);
  points.emplace_back(spawn_point_index.x+offset*sign,spawn_point_index.y-offset*sign);
  // bot
  std::vector<sf::Vector2f> dir_bot = {{sign*1,0},{0,sign*1}};
  if(side == Side::RED)std::reverse(dir_top.begin(), dir_top.end());
  directions.push_back(dir_bot);
  points.emplace_back(spawn_point_index.x,spawn_point_index.y-offset*sign);

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
    champ->round_end();
  }
}
void Champion::add_gamemove(GameMove *move){
  gamemoves.push_back(move);
  current_gamemove = gamemoves.back();
}
void Champion::round_end(){
  movepoints = 3;
  current_gamemove = nullptr;
  for(GameMove *move : gamemoves){
    delete move;
  }
  gamemoves.clear();
  simulation_points_counter = 1;
  // passive gold generation
  gold+=20;

  // check if any wards expired and remove them
  for(auto iter = wards.begin(); iter != wards.end(); iter++){
    if(!(*iter)->isAlive()){
      // erase returns the next element after the erased one,
      // this way we can delete elements while looping through the vector
      iter = wards.erase(iter);
    }
  }
}
void Champion::update_total_dmg(){
  double total_dmg_ = get_base_dmg();
  for(auto &item: items){
    total_dmg_+= item->get_bonus_dmg();
  }
  total_damage = total_dmg_;
}
void Champion::update_total_hp(){
  double total_hp_ = get_base_hp();
  for(auto &item: items){
    total_hp_+= item->get_bonus_hp();
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
  sf::Vector2f current_pos = cell->get_index();
  for(size_t i = 0; i < directions.size()-1; i++){
    if(directions[i] == current_pos){
      return directions[i+1];
    }
  }
  return !directions.empty() ? directions[0] : sf::Vector2f{0, 0};
}
Minion::Minion(Side side_, std::vector<sf::Vector2f> directions_, Cell *spawn_point){
  directions = directions_;
  side = side_;
  cell = spawn_point;
  set_color(sf::Color{100,165,90});
  this->set_name("minion");
  this->hp = 30;
  this->damage = 30;
  this->shape.setSize({10,10});
}
void Player::spawn_champs(const std::shared_ptr<Map> &map){
  for(auto & champ : champs){
    sf::Vector2f spawn_point_index = spawn_point->get_index();
    map->spawn(champ,spawn_point_index);
    champ->set_cell(map->getcell(spawn_point_index));
  }
}
void Player::do_moves(std::shared_ptr<Map> map){
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
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->set_font(holder);
  }
}
void Champion::set_font(Resources::Holder &h){
  icon.setFont(h.get(Resources::Type::FONT));
}
bool Entity::clicked(const int x, const int y){
  return shape.getGlobalBounds().contains(static_cast<float>(x),static_cast<float>(y));
}
std::string Entity::to_ui_int_format(double num)const{
  return std::to_string(static_cast<int>(num));
}
std::vector<std::string> Entity::get_stats()const{
  std::vector<std::string> stats;
  stats.push_back("name: "+name);
  stats.push_back("max_hp: "+to_ui_int_format(max_hp));
  stats.push_back("hp: "+to_ui_int_format(hp));
  stats.push_back("dmg: "+to_ui_int_format(damage));
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
  set_side(Side::NEUTRAL);
  shape.setFillColor(sf::Color{130,100,230});
}
Nexus::Nexus(){
  name = "nexus";
  max_hp = 300;
  hp = 300;
  damage = 0;
  shape.setFillColor(sf::Color{100,50,88});
}
bool Player::is_his_champ(Champion *c){
  if(c == nullptr)return false;
  for(size_t i = 0; i < champs.size(); i++){
    if(champs[i]->get_name() == c->get_name())return true;
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
    else current_gamemove = nullptr;
  }
}

Champion *Player::get_selected_champs(sf::Vector2f index){
  for(int i = static_cast<int>(champs.size())-1; i >=0; i--){
    size_t current_index = static_cast<size_t>(i);
    if(champs[current_index]->get_simulation_cell()->get_index() == index)return champs[current_index];
  }
  return nullptr;
}
bool Player::is_gamemove_active() const{
  return gamemoveactive;
}
void Player::set_gamemove_active(bool active){
  gamemoveactive = active;
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
void Player::set_side(Side side_){
  side = side_;
  for(Champion *champ:champs){
    champ->set_side(side_);
  }
}
void Tower::attack(std::shared_ptr<Map> &map){
  // checks if there are nearby enemies, and attacks them
  std::vector<Cell *> nearby_cells = map->getnearbycells(cell->get_index());
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
      e.set_bonus_dmg(10);
      setEffect(e);
      break;
    }
    case 1:{
      e.set_bonus_hp(10);
      setEffect(e);
      break;
    }
  }
}
void Champion::place_ward(std::shared_ptr<Map> map, Cell *c){
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
  update_total_dmg();
  update_total_hp();
}
sf::Vector2f Champion::gamemove_index(size_t offset)const{
  if(!current_gamemove)throw std::runtime_error("current game move is a nullptr");
  int last_index = static_cast<int>(gamemoves.size()) - 1;
  for(int i = last_index - static_cast<int>(offset); i >= 0; i--){
    size_t index = static_cast<size_t>(i);
    if(gamemoves[index]->position_cell() != nullptr){
      return gamemoves[index]->position_cell()->get_index();
    }
  }
  if(cell == nullptr)return {-1,-1};
  return cell->get_index();
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
    size_t index = static_cast<size_t>(i);
    if(gamemoves[index]->position_cell() != nullptr && gamemoves[index]->changes_pos()){
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
void MinionWave::spawn(sf::Vector2f startpoint,std::vector<sf::Vector2f> directions_, std::shared_ptr<Map> map, Side side_){
  directions = directions_;
  for(size_t i = 0; i < minion_wave_size; i++){
    Minion *minion = new Minion{side_,directions_,map->getcell(startpoint)};
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
void Champion::do_move(std::shared_ptr<Map> map){
  if(!isAlive()){
    respawn_counter--;
    if(respawn_counter == 0){
      hp = max_hp;
      if(spawn_point!=nullptr){
        cell = spawn_point;
      }
    }
    return;
  }
  if(!gamemoves.empty()){
    GameMove *first = *gamemoves.begin();
    if(first->get_movepoints() == simulation_points_counter && first->is_complete()){
      first->do_move(this,map);
      delete first;
      gamemoves.erase(gamemoves.begin());
      simulation_points_counter = 1;
    }else{
      simulation_points_counter++;
    }
  }
  for(size_t i = 0; i < wards.size(); i++){
    wards[i]->do_move();
  }
}
void MinionWave::do_move(const std::shared_ptr<Map> &map){
  for(auto minion: minions){
    minion->do_move(map);
  }
}
void Minion::do_move(const std::shared_ptr<Map> &map){
  sf::Vector2f next_cell_dir = this->get_next_direction_pos_index();
  // Todo: if it cannot go in that direction, then go to next direction,
  // or if no direction is available then just go
  sf::Vector2f next_cell_index = cell->get_index()+next_cell_dir;
  //if(map->in_bounds(next_cell_index)){

  //}
  Cell *next_cell = map->getcell(next_cell_index);

  // if there are enemies on the next cell the minion wants to go to, then it attacks the enemy
  Entity *enemy = next_cell->get_attackable_entity(side);
  if(enemy != nullptr){
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
  for(size_t i = 0; i < champs.size(); i++){
    champs[i]->set_simulation(sim);
  }
  is_simulation = sim;
}
void Player::update_champ_positions(std::shared_ptr<Map> map){
  for(size_t i = 0; i < champs.size(); i++){
    if(is_simulation){
      map->move(champs[i], champs[i]->get_real_cell()->get_index(), champs[i]->get_simulation_cell()->get_index());
    }else{
      map->move(champs[i], champs[i]->get_simulation_cell()->get_index(), champs[i]->get_real_cell()->get_index());
    }
  }
}
void Champion::fight(Entity *other){
  if(!isAlive() || !other->isAlive())return;
  std::cout << "fight " << std::endl;
  double total_dmg = get_total_dmg();
  double other_total_dmg = other->get_total_dmg();
  remove_hp(other_total_dmg);
  other->remove_hp(total_dmg);
  if(!other->isAlive()){
    Effect effect = other->get_effect_if_killed();
    if(effect.not_zero()){
      buffs.push_back(effect);
    }
  }
}
void Champion::fight(Champion *other){
  double total_dmg = get_total_dmg();
  double total_hp = get_total_hp();
  double other_total_dmg = other->get_total_dmg();
  double other_total_hp = other->get_total_hp();
  // if one of them can kill the other
  if(total_dmg >= other_total_hp || other_total_dmg >= total_hp){
    double chance = ((total_dmg+other_total_dmg)/total_dmg + (total_hp+other_total_hp)/total_hp) /2;
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
    hp+=hp_per_level;
    level++;
    xp_cutoff += level_xp_increase;
    update_total_dmg();
    update_total_hp();
  }
}