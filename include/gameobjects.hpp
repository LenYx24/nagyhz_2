#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP
#include "gamemoves.hpp"
#include "ioparser.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
class Cell;

enum class Side { BLUE, RED };
// Todo: move this file into seperate smaller files later
struct Stat {
  int killcount;
  // int assistcount; depends on how i implement fighting
  int deathcount;
};
class Entity {
public:
  Entity(std::string name = "");
  // resets its hp to full
  virtual void spawn() = 0;
  virtual void die() = 0;
  inline bool isAlive() const {
    return respawn_timer == 0;
  }
  // checks
  virtual void updatevision() = 0;

protected:
  std::string name;
  double maxhp; // the maximum hp this entity could have
  double hp;
  double damage;
  int respawn_timer; // the amount of seconds needed to respawn
  Cell *cells;       // pointer to the cell this entity occupies
  // Todo: implement the correct sequence in which entities get drawn to the map, by utilizing the z-index
  // Todo: use sprite with/instead of color coding entities
  sf::Color color;
};
class Effect {
protected:
  double bonusdmg;
  double bonushp;
};
class Buff : public Effect {
  // a buff is a kind of effect given by slaying a monster, which could have a timer on it, that shows how long its active
};
class Item : public Effect {
protected:
  int gold_value;
  // extra props:
  // is it a percentage bonus, or direct value
  // callback fnc, that does something to its champion, or on the map
};
class Champion : public Entity, public Ireadstring {
  // spawns the champion with full health on the side he's on
  void spawn();
  void die();
  bool fight(Champion &other); // simulates a fight, by calculating each ones dmg
  double gettotaldmg();        // returns the total dmg that could be dealt by the champion with all the buffs and items
  void dogamemoves();          // iterates through the gamemoves vector, does every move, then empties the vector
  // tries to add an item to the champion, returns true if its successful
  // and check if the champions position is in its own base cell
  // if thats true, then check if the champion has enough gold to buy the item
  // checks if the champions inventory is not full
  bool additem(Item *item);
  void updatevision();

  virtual void readfromstring(std::string &line, const char delimiter);

protected:
  bool enoughgold(int gold); // returns true, if the champion has more or the same gold given in the arguments
  bool isinventoryfull();    // checks if the champions inventory is full
  bool inbase();             // checks if the current cell is the base cell for this champion
  int cs;
  int gold;
  int hp_per_level;  // the amount of hp given per level up
  int dmg_per_level; // the amount of dmg given per level up
  Item items[6];
  int level; // by leveling up the basic attributes of a champion get multiplied by level_multiplier
  double level_multiplier;
  int xp;        // the current amount of xp
  int xp_cutoff; // the amount of xp needed to
  int xp_given;  // the xp given to the other champion, if this one gets slain by them
  int vision_range;
  int wards;          // starts from 0, goes to the max value of 2
  int wards_max;      // default is 2
  int wards_cooldown; // default should be 4 rounds
  // exra props:
  // role (enum)
  std::vector<GameMove> gamemoves; // stores the added gamemoves in a turn
};
class Structure {
  // common parent class for entities, it shouldn't have a move functions, it's position doesn't change
};
class Tower : public Structure {
  // selects its target, from targetable classes, these are champions, and minions (monsters cannot reach)
  // prioritizes minions in its range, and attacks them, instead of champions
  void selection();

protected:
  int range; // the amount of cells it can damage to in an area
};
class Nexus : public Structure {
  // doesn't do damage, it's only stationary
  // when it dies, the game ends, so it pops the current game state, and it saves the game
};
class Monster : public Entity {
  // a common class for entities, which are not managed by the player, they can attack champions, and champions can slain them
protected:
  int xp_given; // the amount of xp given to the champion by slaying them
};
class Camp : Monster {
  // a common class for monsters, which are not able to move (baron nashor, drakes and jungle camps)
  // because of how the game works, every camp can give an effect to the champion(s) that slain it
protected:
  Effect effect;
};
// minions can have effects too (currently one, but it could increase in the future), this is the baron buff, which gives flat buffs
class Minion : Monster {

protected:
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
  // loops through its champions and instructs them to do the moves
  void domoves();

private:
  Champion champs[5];
  Side side;
};
#endif
