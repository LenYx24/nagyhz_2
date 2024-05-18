#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP
#include "gamemoves.hpp"
#include "ioparser.hpp"
#include "map.hpp"
#include "resources.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <memory>
#include <utility>
class Cell;
class Map;
enum class Side { BLUE, RED };
// Todo: move this file into seperate smaller files later
struct Stat {
  int killcount;
  // int assistcount; depends on how i implement fighting
  int deathcount;
};
class Entity {
public:
  virtual ~Entity(){}
  Entity(std::string name = "");
  virtual void draw(sf::RenderWindow &w);
  // resets its hp to full
  //virtual void spawn() = 0;
  //virtual void die() = 0;
  double get_base_dmg()const{return damage;}
  double get_base_hp()const{return hp;}
  double get_total_dmg()const{return total_damage;}
  double get_total_hp()const{return total_hp;}
  int get_xp_given()const{return xp_given;}
  int get_gold_given()const{return gold_given;}
  void set_side(Side side_){side = side_;}
  Side get_side()const {return side;}
  void set_xp_given(int xp_given_){xp_given = xp_given_;}
  virtual std::vector<std::string> get_stats();
  bool isAlive() const {return respawn_timer == 0;}
  virtual bool should_focus()const{return false;}
  virtual bool gives_creep_score()const {return false;}
  virtual void remove_hp(double dmg){if(get_total_hp()-dmg > 0)hp-=dmg; check_death();}
  virtual void check_death(){if(total_hp <= 0){respawn_counter = respawn_timer;}}
  virtual bool clicked(int, int);
  // the cell where the entity is at the start of the round
  inline Cell *get_real_cell(){return cell;}
  virtual // the cell which calculates in the gamemoves of the current cell
  inline Cell *get_simulation_cell(){return cell;}
  inline void set_cell(Cell *c){if(c != nullptr)cell=c;}
  virtual void update_shape_pos(sf::Vector2f pos);
  virtual bool gives_vision()const{return false;}
  // checks
  // virtual void update_vision() = 0;
  std::string to_ui_int_format(double num);
  void set_color(sf::Color color_){shape.setFillColor(color_);}
  void set_name(std::string name_){name = std::move(name_);}
  virtual bool can_fight_back()const{return false;}
protected:
  std::string name;
  double max_hp = 10; // the maximum hp this entity could have
  double hp = 10;
  double damage = 10;
  double total_hp = 10;
  double total_damage = 10;
  int respawn_counter; // the amount of seconds needed to respawn
  int respawn_timer; // the amount of seconds needed to respawn
  int xp_given = 10;  // the xp given to the other entity, if this one gets slain by them
  int gold_given;  // the gold given to the other entity, if this one gets slain by them
  Cell *cell;       // pointer to the cell this entity occupies
  Side side; // the side on which the entity is on, either blue or red
  sf::Color color;
  sf::RectangleShape shape;
};
class Effect {
public:
  Effect():bonusdmg(0),bonushp(0){}
  Effect(int dmg, int hp):bonusdmg(dmg),bonushp(hp){}
  double getbonusdmg()const{return bonusdmg;}
  double getbonushp()const{return bonushp;}
  void setbonusdmg(double bonusdmg){this->bonusdmg = bonusdmg;}
  void setbonushp(double bonushp){this->bonushp = bonushp;}
private:
  double bonusdmg;
  double bonushp;
};
class Buff : public Effect {
  // a buff is a kind of effect given by slaying a monster, which could have a timer on it, that shows how long its active
};
class Item : public Effect {
public:
  void readfromstring(std::string &line, const char delimiter=';');
  int get_gold_value()const{return gold_value;}
  inline std::string getname()const{return name;}
private:
  int gold_value;
  std::string name;
  // extra props:
  // is it a percentage bonus, or direct value
  // callback fnc, that does something to its champion, or on the map
};
/**
 * @brief common parent class for structures, it shouldn't have a move (as in map movements) functions, it's position doesn't change
 */
class Structure : public Entity {
  bool gives_vision()const override{return true;}
};
class Ward: public Structure{
public:
  Ward():cooldown(5){set_color(sf::Color{25,25,25});}
private:
  int cooldown;
};
class Champion : public Entity, public Ireadstring {
public:
  Champion();
  ~Champion() override;
  void fight(Entity *other); // simulates a fight, by calculating each ones dmg
  void update_total_dmg(); // returns the total dmg that could be dealt by the entity with all the buffs and items
  void update_total_hp(); // returns the total hp that this entity has with all the buffs and items
  void add_item(Item *item);
  std::vector<std::string> get_stats();
  inline void set_icon(char c){icon.setString(c);}
  std::string get_name() const {return name;}
  void set_font(Resources::Holder &h);
  void read_from_string(std::string &line, const char delimiter = ';');
  virtual void draw(sf::RenderWindow &w);
  int getmovepoints()const{return movepoints;}
  void add_gamemove(GameMove *move){gamemoves.push_back(move); current_gamemove = gamemoves[gamemoves.size()-1];}
  bool is_gamemove_complete()const{if(gamemoves.empty())return true; else return current_gamemove->is_complete();}
  Cell *get_simulation_cell();
  sf::Vector2f last_gamemove_index()const;
  sf::Vector2f current_gamemove_index()const;
  void finish_gamemove(Cell *cell);
  void update_shape_pos(sf::Vector2f pos)override;
  void do_move(std::shared_ptr<Map> map);
  void set_simulation(bool sim){simulation = sim;}
  void round_end();
  void add_xp(int xp);
  bool can_fight_back()const override{return true;}
  void place_ward(std::shared_ptr<Map> map, Cell *c);
  bool gives_vision()const override{return false;}
  void refill_hp(){hp = max_hp;}
  void clear_gamemoves();

private:
  sf::Vector2f gamemove_index(size_t offset)const;
  inline bool enough_gold(int gold_){return gold >= gold_;} // returns true, if the champion has more or the same gold given in the arguments
  inline bool isinventory_full(){return items.size() == 6;}    // checks if the champions inventory is full
  inline bool in_base(){return true;}             // checks if the current cell is the base cell for this champion
  int cs = 0;
  int gold = 0;
  int hp_per_level = 0;  // the amount of hp given per level up
  int dmg_per_level = 0; // the amount of dmg given per level up
  std::vector<Item *> items;
  int level = 1; // the current level of the champion
  int max_level = 18;
  int level_xp_increase = 10; // the amount of xp which is added to xp_cutoff at every levelup
  int xp = 0;        // the current amount of xp
  int xp_cutoff = 100; // the amount of xp needed to level up
  std::vector<Ward *> wards;
  size_t wards_max = 2;      // default is 2

  int movepoints = 0;
  sf::Text icon;
  std::vector<GameMove *> gamemoves; // stores the added gamemoves in a turn
  GameMove *current_gamemove;
  int simulation_points_counter = 1;
  bool simulation = false;
};

class Tower : public Structure {
public:
  Tower();
  void attack(std::shared_ptr<Map> &map);
};
class Nexus : public Structure {
  public:
  Nexus();
  // doesn't do damage, it's only stationary
  // when it dies, the game ends, so it pops the current game state, and it saves the game
};
class Monster : public Entity {
  // a common class for entities, which are not managed by the player, they can attack champions, and champions can slain them
private:
  int xp_given; // the amount of xp given to the champion by slaying them
  int gold_given;
};
// a common class for monsters, which are not able to move (baron nashor, drakes and jungle camps)
// because of how the game works, every camp can give an effect to the champion(s) that slain it
class Camp : public Monster {
public:
  Camp();
  
  void setEffect(Effect e){effect = e;}
private:
  Effect effect;
};
class Drake : public Camp{
public:
  Drake();
  void decide_which_type();
};
// minions can have effects too (currently one, but it could increase in the future), this is the baron buff, which gives flat buffs
class Minion : public Monster {
public:
  Minion();
  virtual bool gives_vision()const{return false;}
  bool should_focus()const override{return true;}
  bool gives_creep_score()const{return true;}
};
class MinionWave{
public:
  MinionWave():minion_wave_size(3){}
  void spawn(sf::Vector2f startpoint,std::vector<sf::Vector2f> directions, std::shared_ptr<Map> map, Side side_);
  // check for minion deaths...
  void round_end();

  // checks if the minion can attack something in front of it, if not then moves forward
  void do_minions_move();
private:
  std::vector<Minion *> minions;
  std::vector<sf::Vector2f> directions;
  sf::Vector2f current_direction;
  size_t minion_wave_size;
};
class Player {
public:
  Player(std::vector<Champion*> champs);
  ~Player();
  void spawn_champs(const std::shared_ptr<Map> &map);
  void setspawnpoint(sf::Vector2f point){spawnpoint = point;}
  void setchampicons(const std::string &icons);
  void setfont(Resources::Holder& h);
  bool is_gamemove_active();
  void setgamemoveactive(bool b);
  void domoves(std::shared_ptr<Map> map);
  bool ishischamp(Champion *c);
  void showmoveoptions(const std::shared_ptr<Map>, Champion *c);
  void draw_champs(sf::RenderWindow &window);
  bool did_start()const{return starter;}
  void set_starter(bool s){starter = s;}
  bool check_round_end();
  void spawn_minions(const std::shared_ptr<Map>& minions);
  void round_end(std::shared_ptr<Map> map);
  void set_simulation(bool sim);
  void update_champ_positions(std::shared_ptr<Map> map);
  void set_side(Side s);
  Side get_side()const{return side;}
  Champion *getselectedchamp(sf::Vector2f index);
  sf::Vector2f get_spawn_point()const{return spawnpoint;}
  void clear_gamemoves();
  void despawn_champs(std::shared_ptr<Map> &map);


private:
  std::vector<Champion*> champs;
  // minions
  std::vector<MinionWave*> minion_waves;
  Side side;
  bool gamemoveactive;
  bool starter;
  sf::Vector2f spawnpoint;
  // save global buffs here, so the minions know, if there's nashor buff
  int minion_timer;
  int minion_timer_mark;
  bool is_simulation;
};
#endif
