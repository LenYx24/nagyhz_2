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
Player::Player(std::vector<Champion*> champs){
  this->champs = champs;
  gamemoveactive = false;
}

void Entity::draw(sf::RenderWindow &w, sf::Vector2f pos){
  shape.setPosition(pos + sf::Vector2f{5,5});
  w.draw(shape);
}
void Champion::draw(sf::RenderWindow &window){
  std::cout << "cellnullptr" << std::endl;
  if(cell != nullptr){
    std::cout << "cellnotnullptr" << std::endl;
    sf::Vector2f pos = cell->get_position();
    shape.setPosition(pos + sf::Vector2f{5,5});
    window.draw(shape);
    icon.setPosition(shape.getPosition() + sf::Vector2f{2,2});
    icon.setCharacterSize(10);
    icon.setFillColor(sf::Color::White);
    window.draw(icon);
  }
}
void Champion::draw(sf::RenderWindow &w, sf::Vector2f pos){
  shape.setPosition(pos + sf::Vector2f{5,5});
  w.draw(shape);
  icon.setPosition(shape.getPosition() + sf::Vector2f{2,2});
  icon.setCharacterSize(10);
  icon.setFillColor(sf::Color::White);
  w.draw(icon);
}
void Champion::setname(std::string name){this->name=name;}
void Player::spawnchamps(const std::shared_ptr<Map> map){
  for(size_t i = 0; i < champs.size(); i++){
    map->spawn(champs[i],spawnpoint);
    champs[i]->setcell(map->getcell(spawnpoint));
  }
}
void Player::domoves(){
  // do moves
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
  this->icon.setFont(h.get(Resources::Type::FONT));
}
bool Entity::clicked(const int x, const int y){
  return shape.getGlobalBounds().contains(x,y);
}
// bool Tower::clicked(const int x, const int y){
//   return true;
// }
bool Champion::clicked(const int x, const int y){
  return icon.getGlobalBounds().contains(x,y);
}
std::string Entity::to_ui_int_format(double num){
  return std::to_string(static_cast<int>(num));
}
std::vector<std::string> Entity::getstats(){
  std::vector<std::string> stats;
  std::cout << "name: " << name << "\nhp: " << hp << std::endl;
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
  for(size_t i = 0; i < champs.size(); i++){
    if(champs[i]->getcell()->getindex() == index)return champs[i];
  }
  std::cout << "returned nullptr as selected champ" << std::endl;
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
  movepoints = 5;
}
sf::Vector2f Champion::gamemove_index(size_t offset)const{
  if(!current_gamemove)throw "current game move is a nullptr";
  size_t lastindex = gamemoves.size() - 1;
  for(size_t i = lastindex - offset; i >= 0; i--){
    if(gamemoves[i]->position_cell() != nullptr){
      return gamemoves[i]->position_cell()->getindex();
    }
  }
  std::cout << "cellgetindex" << std::endl;
  return cell->getindex();
}
sf::Vector2f Champion::last_gamemove_index()const{
  return gamemove_index(0);
}
sf::Vector2f Champion::current_gamemove_index()const{
  return gamemove_index(1);
}
