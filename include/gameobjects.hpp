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
  virtual void draw(sf::RenderWindow &w, sf::Vector2f pos);
  // resets its hp to full
  //virtual void spawn() = 0;
  //virtual void die() = 0;
  virtual std::vector<std::string> getstats();
  inline bool isAlive() const {
    return respawn_timer == 0;
  }
  virtual bool clicked(const int, const int);
  inline Cell *getcell(){return cell;}
  inline void setcell(Cell *c){cell=c;}
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
  // spawns the champion with full health on the side he's on
  void spawn();
  void die();
  bool fight(Champion &other); // simulates a fight, by calculating each ones dmg
  double get_total_dmg();      // returns the total dmg that could be dealt by the champion with all the buffs and items
  void do_game_moves();        // iterates through the gamemoves vector, does every move, then empties the vector
  // tries to add an item to the champion, returns true if its successful
  // and check if the champions position is in its own base cell
  // if thats true, then check if the champion has enough gold to buy the item
  // checks if the champions inventory is not full
  bool add_item(Item *item);
  void setname(std::string name);
  void update_vision();
  std::vector<std::string> getstats();
  inline void seticon(char c){icon.setString(c);}
  std::string getname() const {return name;}
  virtual bool clicked(const int, const int);
  ~Champion();
  void setfont(Resources::Holder &h);
  void readfromstring(std::string &line, const char delimiter = ';');
  virtual void draw(sf::RenderWindow &w);
  virtual void draw(sf::RenderWindow &w, sf::Vector2f pos);
  int getmovepoints()const{return movepoints;}
  void add_gamemove(GameMove *move){gamemoves.push_back(move); current_gamemove = gamemoves[gamemoves.size()-1];}
  bool is_gamemove_complete()const{if(gamemoves.size() == 0)return false; else return current_gamemove->is_complete();}
  bool can_move(int p){
    bool b = movepoints-p>=0;
    if(b)movepoints-=p; 
    return b;
  }
  sf::Vector2f last_gamemove_index()const;
  sf::Vector2f current_gamemove_index()const;
  void finish_gamemove(Cell *cell);

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
  int xp_cutoff; // the amount of xp needed to
  int xp_given;  // the xp given to the other champion, if this one gets slain by them
  int vision_range;
  int wards;          // starts from 0, goes to the max value of 2
  int wards_max;      // default is 2
  int wards_cooldown; // default should be 4 rounds

  int movepoints;
  sf::Text icon;
  std::vector<GameMove *> gamemoves; // stores the added gamemoves in a turn
  GameMove *current_gamemove;
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
protected:
  int xp_given; // the amount of xp given to the champion by slaying them
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
  Side side; // on which side the minion is on
  // Todo: add game moves for minions
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
  void domoves();
  bool ishischamp(Champion *c);
  void showmoveoptions(const std::shared_ptr<Map>, Champion *c);
  void draw_champs(sf::RenderWindow &window);
  Champion *getselectedchamp(sf::Vector2f index);

private:
  std::vector<Champion*> champs;
  Side side;
  bool gamemoveactive;
  sf::Vector2f spawnpoint;
};
#endif
