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
  virtual std::vector<std::string> getstats();
  inline bool isAlive() const {
    return respawn_timer == 0;
  }
  virtual bool clicked(const int, const int);
  // the cell where the entity is at the start of the round
  inline Cell *get_real_cell(){return cell;}
  // the cell which calculates in the gamemoves of the current cell
  inline Cell *get_simulation_cell(){return cell;}
  inline void setcell(Cell *c){if(c != nullptr)cell=c;}
  virtual void update_shape_pos(sf::Vector2f pos);
  // checks
  // virtual void update_vision() = 0;
  std::string to_ui_int_format(double num);
protected:
  std::string name;
  double maxhp; // the maximum hp this entity could have
  double hp;
  double damage;
  int respawn_timer; // the amount of seconds needed to respawn
  Cell *cell;       // pointer to the cell this entity occupies
  // Todo: implement the correct sequence in which entities get drawn to the map, by utilizing the z-index
  // Todo: use sprite with/instead of color coding entities
  sf::Color color;
  sf::RectangleShape shape;
};
class Effect {
public:
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
class Champion : public Entity, public Ireadstring {
public:
  Champion();
  ~Champion();
  // spawns the champion with full health on the side he's on
  void spawn();
  void die();
  bool fight(Champion &other); // simulates a fight, by calculating each ones dmg
  double get_total_dmg();      // returns the total dmg that could be dealt by the champion with all the buffs and items
  void add_item(Item *item);
  void setname(std::string name);
  void update_vision();
  std::vector<std::string> getstats();
  inline void seticon(char c){icon.setString(c);}
  std::string getname() const {return name;}
  void setfont(Resources::Holder &h);
  void readfromstring(std::string &line, const char delimiter = ';');
  virtual void draw(sf::RenderWindow &w);
  int getmovepoints()const{return movepoints;}
  void add_gamemove(GameMove *move){gamemoves.push_back(move); current_gamemove = gamemoves[gamemoves.size()-1];}
  bool is_gamemove_complete()const{if(gamemoves.size() == 0)return true; else return current_gamemove->is_complete();}
  Cell *get_simulation_cell();
  sf::Vector2f last_gamemove_index()const;
  sf::Vector2f current_gamemove_index()const;
  void finish_gamemove(Cell *cell);
  virtual void update_shape_pos(sf::Vector2f pos);
  void do_move(std::shared_ptr<Map> map);
  void set_simulation(bool sim){simulation = sim;}
  void round_end();

private:
  sf::Vector2f gamemove_index(size_t offset)const;
  inline bool enough_gold(int gold){return this->gold >= gold;} // returns true, if the champion has more or the same gold given in the arguments
  inline bool isinventory_full(){return items.size() == 6;}    // checks if the champions inventory is full
  inline bool in_base(){return true;}             // checks if the current cell is the base cell for this champion
  int cs;
  int gold;
  int hp_per_level;  // the amount of hp given per level up
  int dmg_per_level; // the amount of dmg given per level up
  std::vector<Item *> items;
  int level; // by leveling up the basic attributes of a champion get multiplied by level_multiplier
  double level_multiplier;
  int xp;        // the current amount of xp
  int xp_cutoff; // the amount of xp needed to level up
  int xp_given;  // the xp given to the other champion, if this one gets slain by them
  int vision_range;
  int wards;          // starts from 0, goes to the max value of 2
  int wards_max;      // default is 2
  int wards_cooldown; // default should be 4 rounds

  int movepoints;
  sf::Text icon;
  std::vector<GameMove *> gamemoves; // stores the added gamemoves in a turn
  GameMove *current_gamemove;
  int simulation_points_counter;
  bool simulation;
};
class Structure : public Entity {
  // common parent class for entities, it shouldn't have a move functions, it's position doesn't change
};
class Tower : public Structure {
public:
  Tower();
  // selects its target, from targetable classes, these are champions, and minions (monsters cannot reach)
  // prioritizes minions in its range, and attacks them, instead of champions
  void selection();

protected:
  int range; // the amount of cells it can damage to in an area
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
class Camp : public Monster {
public:
  Camp();
  // a common class for monsters, which are not able to move (baron nashor, drakes and jungle camps)
  // because of how the game works, every camp can give an effect to the champion(s) that slain it
public:
  inline void setEffect(Effect e){effect = e;}
private:
  Effect effect;
};
// minions can have effects too (currently one, but it could increase in the future), this is the baron buff, which gives flat buffs
class Minion : public Monster {

private:
  // Todo: add game moves for minions
};
class MinionWave{
public:
  MinionWave():minion_wave_size(3){}
  void spawn(sf::Vector2f startpoint, std::shared_ptr<Map> map);
  // check for minion deaths...
  void round_end();

  // checks if the minion can attack something in front of it, if not then moves forward
  void do_minions_move();
private:
  std::vector<Minion *> minions;
  size_t minion_wave_size;
};
class Ward : public Entity {
public:
protected:
  int radius; // the amount of cells this one gets vision for
};
class Player {
public:
  Player(std::vector<Champion*> champs);
  void spawnchamps(const std::shared_ptr<Map> map);
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
  void spawn_minions(std::shared_ptr<Map> minions);
  void round_end(std::shared_ptr<Map> map);
  void set_simulation(bool sim);
  void update_champ_positions(std::shared_ptr<Map> map);
  void set_side(Side s){side = s;}
  Champion *getselectedchamp(sf::Vector2f index);
  sf::Vector2f get_spawn_point()const{return spawnpoint;}

private:
  std::vector<Champion*> champs;
  Side side;
  bool gamemoveactive;
  bool starter;
  sf::Vector2f spawnpoint;
  // save global buffs here, so the minions know, if there's nashor buff
  // minions
  std::vector<MinionWave*> minion_waves;
  int minion_timer;
  int minion_timer_mark;
  bool is_simulation;
};
#endif
