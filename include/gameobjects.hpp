#ifndef GAMEOBJECTS_HPP
#define GAMEOBJECTS_HPP
#include "gamemoves.hpp"
#include "map.hpp"
#include "resources.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <sstream>
#include <list>
#include <memory>
#include <utility>
class Cell;
class Map;
class Effect {
public:
  /**
    * @brief contructor which set's bonus_dmg and bonus_hp to their respective values
    * @param dmg the new damage to use
    * @param hp the new base_hp to use
   */
  explicit Effect(int dmg=0, int hp=0): bonus_dmg(dmg), bonus_hp(hp){}
  /**
    * @brief get's the bonus_dmg
   */
  [[nodiscard]] double get_bonus_dmg()const{return bonus_dmg;}
  /**
    * @brief get's the bonus_hp
   */
  [[nodiscard]] double get_bonus_hp()const{return bonus_hp;}
  /**
    * @brief set's bonus_dmg
   */
  void set_bonus_dmg(double bonus_dmg_){bonus_dmg = bonus_dmg_;}
  /**
    * @brief set's bonus_hp
   */
  void set_bonus_hp(double bonus_hp_){bonus_hp = bonus_hp_;}
  /**
   * checks if the two properties are zero, or not
   * @return true if both of them aren't zero
   */
  [[nodiscard]] bool not_zero()const{return bonus_dmg != 0 && bonus_hp != 0;}
private:
  double bonus_dmg;
  double bonus_hp;
};
/**
 * @brief the enum that holds which team the entity is on
 */
enum class Side { BLUE, RED, NEUTRAL };
/**
 * @brief The class that describes an entity
 */
class Entity {
public:
  /**
    * @brief entity's default destructor
   */
  virtual ~Entity()= default;
  /**
    * @brief entity's constructor
    * @param name the name of the entity
   */
  explicit Entity(std::string name = "");
   /**
    * @brief the method that draws the entity to the window
    * @param window the window to draw to
    */
  virtual void draw(sf::RenderWindow &window);
  /**
    * @brief returns the base damage of the champion, without any buffs/items
    * only leveling up changes the base value
   */
  double get_base_dmg()const{return damage;}
  /**
    * @brief returns the base base_hp, works the same way as get_base_dmg
   */
  double get_base_hp()const{return base_hp;}
  /**
    * @brief returns the base base_hp, works the same way as get_base_dmg
   */
  virtual double get_max_hp()const{return base_hp;}
  /**
    * @brief returns the total damage, by adding buffs/items to the base dmg
   */
  double get_total_dmg()const{return total_damage;}
  /**
    * @brief returns the amount of experience given to the entity that kills this entity
   */
  int get_xp_given()const{return xp_given;}
  /**
    * @brief returns the amount of gold given to the entity that kills this entity
   */
  int get_gold_given()const{return gold_given;}
  /**
    * @brief set's which team(side) is the entity on
   */
  void set_side(Side side_){side = side_;}
  /**
    * @brief gets the current team the entity is on
   */
  Side get_side()const {return side;}
  /**
    * @brief set's the amount of xp this entity could give
   */
  void set_xp_given(int xp_given_){xp_given = xp_given_;}
  /**
    * @brief gets this entity's statistics that could describe it
   */
  virtual std::vector<std::string> get_stats() const;
  /**
    * @brief checks if the entity is alive currently
   */
  bool is_alive() const {return alive;}
  /**
    * @brief returns true, if this entity should be focused by other entities when trying to pick a fight
   */
  virtual bool should_focus()const{return false;}
  /**
    * @brief checks if this entity increases the creep score of the other entity when killed
   */
  virtual bool gives_creep_score()const {return false;}
  /**
    * @brief removes the given damage from the entity's total base_hp, and checks if the entity died by this damage
    * @param dmg the amount of damage dealt to this entity
   */
  void remove_hp(double dmg);
  /**
    * @brief checks if the entity died
   */
  void check_death();
  /**
    * @brief checks if the entity's shape was clicked on
   */
  virtual bool clicked(int x, int y);
  /**
    * @brief returns the cell on which the entity is at every start of the round
   */
  Cell *get_real_cell(){return cell;}
  /**
    * @brief returns the cell on which the entity advances to with gamemoves during a round
   */
  virtual Cell *get_simulation_cell(){return cell;}
  /**
    * @brief set's this entity's cell, it should be a valid cell on the map
   */
  void set_cell(Cell *c){if(c != nullptr)cell=c;}
  /**
    * @brief update's the shape's position so it appears on it's cell
   */
  virtual void update_shape_pos(sf::Vector2f pos);
  /**
    * @brief return true if this entity should give vision around him
   */
  virtual bool gives_vision()const{return false;}
  /**
    * @brief changes the given num to the format which should be used on the ui
    * @param num the number to convert
    * @return the string which should be drawn to the screen
   */
  static std::string to_ui_int_format(double num);
  /**
    * @brief set's this entity's shape fillcolor to the given color
    * @param color the color to use
   */
  void set_color(sf::Color color_){shape.setFillColor(color_);}
  /**
    * @brief set's the entitiy's name
    * @param name the name to use instead
   */
  void set_name(std::string name_){name = std::move(name_);}
  /**
    * @brief returns true, if this is an entity that can fight back
   */
  virtual bool can_fight_back()const{return false;}
  /**
   * @brief if this entity killed another, then this method should be called
   * @param entity
   */
  virtual void killed_other(Entity *entity);
  /**
   * @brief returns the effect of this entity, which is given if it's killed
   * @return the effect
   */
  virtual Effect get_effect_if_killed()const{return Effect{0,0};}
  virtual void attack(Map *map);
protected:
  std::string name;

  bool alive = true;

  double base_hp = 10; // the base hp without any buffs
  double current_hp = 10; // this is the hp that the entity "uses", this is the one that takes damage
  double damage = 10;
  double total_hp = 10;
  double total_damage = 10;

  int respawn_counter = 0; // the amount of seconds needed to respawn
  int respawn_timer = 3; // the amount of seconds needed to respawn
  int xp_given = 10;  // the xp given to the other entity, if this one gets slain by them
  int gold_given = 30;  // the gold given to the other entity, if this one gets slain by them

  Cell *cell = nullptr;       // pointer to the cell this entity occupies
  Side side = Side::BLUE; // the side on which the entity is on, either blue or red

  sf::Color color;
  sf::RectangleShape shape;
};

/**
 * @brief the class that describes items, which are primarily used to give bonuses to champions
 * (could be used on entities too if needed)
 */
class Item : public Effect {
public:
  Item(std::string name_, int gold_, double bonus_dmg_, double bonus_hp_);
  /**
    * @brief gets the amount of gold needed to purchase this item from the shop
   */
  [[nodiscard]] int get_gold_value()const{return gold_value;}
  /**
    * @brief gets this items name
   */
   [[nodiscard]] std::string get_name()const{return name;}
private:
  int gold_value;
  std::string name;
};
/**
 * @brief common parent class for structures, it shouldn't have a move (as in map movements) functions, it's position doesn't change
 */
class Structure : public Entity {
  /**
    * @brief returns true if the current entity gives vision to its surrounding area
   */
  bool gives_vision()const override{return true;}
};
/**
  * @brief The ward is a type of structure (as it cannot move), that gives vision,
  * but expires after a given time intervall
 */
class Ward: public Structure{
public:
  /**
    * @brief default contsructor
    * initializes the ward's cooldown and it's color
   */
  Ward():cooldown(5){set_color(sf::Color{25,25,25});}
  void do_move();
private:
  int cooldown; // the amount of rounds needed for the ward to disappear
};
/**
  * @brief class for describing champions, they're a type of entities that the players can manipulate with gamemoves
 */
class Champion : public Entity{
public:
  /**
    * @brief constructs a basic champion with all the necessary values
   */
  Champion();
  /**
   * @brief constructing a champion with all its necessary properties
   * @param name_
   * @param damage_
   * @param dmg_per_level_
   * @param hp_
   * @param hp_per_level_
   */
   Champion(const std::string& name_, double damage_, double dmg_per_level_, double hp_, double hp_per_level_);
  /**
    * @brief destructor for champion class, frees the heap allocated properties
   */
  ~Champion() override;
  /**
    * @brief this champion fights another entity, calculates who won, then decreases both entities base_hp
    * gives the required assets to the appropriate entites (such as gold, xp, cs)
    * @param other the entity to fight
   */
  void fight(Entity *other);
  /**
    * @brief this champion fights another champion, calculates who won, then decreases both entities base_hp
    * gives the required assets to the appropriate entites (such as gold, xp, cs)
    * @param other the entity to fight
   */
  void fight(Champion *other);
  /**
    * @brief updates the total dmg that could be dealt by the entity with all the buffs and items
  */
  void update_total_dmg();
  /**
    * @brief updates the total base_hp that could be dealt by the entity with all the buffs and items
   */
  void update_total_hp();
  /**
    * @brief adds an item to the champions item list if the required criteria is met
    * @param item the one to add to the list
   */
  void add_item(Item *item);
  /**
    * @brief gets this champion's new statistics, but also calls the parent for it's statistics
   */
  std::vector<std::string> get_stats()const override;
  /**
    * @brief set's the icon for this champion
    * @param c the char to use
   */
   void set_icon(char c){icon.setString(c);}

   /**
    * @brief gets the champions name
    */
  std::string get_name() const {return name;}
  /**
    * @brief set's the font face for the text
    * @param holder the object that let's you retrieve the font face
   */
  void set_font(Resources::Holder &holder);
  /**
    * @brief draws the champion to the window
    * @param window the window to draw to
   */
  void draw(sf::RenderWindow &window)override;
  /**
    * @brief gets the currently available movepoints
   */
  int getmovepoints()const{return movepoints;}
  /**
    * @brief add's a gamemove to the champions gamemove list
    * @param move the gamemove to add
   */
  void add_gamemove(GameMove *move);
  /**
    * @brief returns the base base_hp, works the same way as get_base_dmg
   */
  double get_max_hp()const override;
  /**
    * @brief check's if the latest gamemove is complete, this means if it can be used up, or it needs some more data
    * @returns true if the gamemove is complete, false if not
   */
  bool is_gamemove_complete()const;
  /**
    * @brief returns the cell on which the champion advances to with gamemoves during a round
   */
  Cell *get_simulation_cell() override;
  /**
    * @brief gets the map position where the last gamemove will act
   */
  sf::Vector2f last_gamemove_index()const;
  /**
    * @brief gets the map position where the current gamemove will act
   */
  sf::Vector2f current_gamemove_index()const;
  /**
    * @brief finishes the last gamemove's setup, by giving it the selected cell
    * @param cell the cell that was selected by the user
   */
  void finish_gamemove(Cell *cell);
  /**
   * @brief removes the last gamemove from this champion
   */
  void remove_last_gamemove();
  /**
    * @brief update the champion's shape position, to match where it should be on the map
    * @param pos the position to change to
   */
  void update_shape_pos(sf::Vector2f pos)override;
  /**
    * @brief does one move from the gamemoves, starting from the first one
    * @param map the map let's it communicate with other entities surrounding it
   */
  void do_move(std::shared_ptr<Map> map);
  /**
    * @brief set's the champions state to the given param, need to know which state it's in while drawing it to the screen
    * @param sim true if its simulation, false if not
   */
  void set_simulation(bool sim){simulation = sim;}
  /**
    * @brief after a round ends (e.g. both players finished their turns) the champion ends it,
    * reset's the necessary variables, prepares for the upcoming round
   */
  void round_end();
  /**
    * @brief adds xp to the champion, and also checks if the champion leveled up with this xp
   */
  void add_xp(int xp);
  /**
    * @brief describes if the champion can fight back another entities
    * Used if this champ has a chance to win, when it's in execute range of the other entity
   */
  bool can_fight_back()const override{return true;}
  /**
    * @brief places a ward on the map, if the given prerequisites are true
    * @param map the map to place the ward on
    * @param cell the cell on the map where the ward should be placed
   */
  void place_ward(const std::shared_ptr<Map>& map, Cell *c);
  /**
    * @brief describes if this entity gives vision
   */
  bool gives_vision()const override{return true;}
  /**
    * @brief refill the base_hp of the champion
   */
  void refill_hp(){current_hp = get_max_hp();}
  /**
    * @brief clears the gamemoves list and also deletes each from the heap, set's the current_gamemove to nullptr
   */
  void clear_gamemoves();
  /**
   * @brief moves the player on the map
   * @param map the map to move on
   */
  void move(std::shared_ptr<Map> &map);
  /**
   * @brief if this entity killed another one
   * @param other the other entity that was killed
   */
  void killed_other(Entity *other) override;
  /**
   * @brief set's the spawnpoint for the champion
   * @param spawn_point_
   */
  void set_spawn_point(Cell *spawn_point_);


private:
  sf::Vector2f gamemove_index(size_t offset)const;
  int cs = 0;
  int gold = 600;
  double hp_per_level = 0;  // the amount of base_hp given per level up
  double dmg_per_level = 0; // the amount of dmg given per level up
  int level = 1; // the current level of the champion
  int max_level = 18;
  int level_xp_increase = 10; // the amount of xp which is added to xp_cutoff at every levelup
  int xp = 0;        // the current amount of xp
  int xp_cutoff = 100; // the amount of xp needed to level up

  int movepoints = 3;

  std::vector<Ward *> wards;
  size_t wards_max = 2;      // default is 2
  std::vector<Item *> items;

  sf::Text icon;
  std::vector<GameMove *> gamemoves; // stores the added gamemoves in a turn
  GameMove *current_gamemove = nullptr;
  int simulation_points_counter = 1;
  bool simulation = false;

  Cell *spawn_point = nullptr;

  std::vector<Effect> buffs;
};
/**
  * @brief the class for a tower, which damages other entities that come near it
 */
class Tower : public Structure {
public:
  /**
    * @brief set's up the towers attributes (base_hp,dmg)
   */
  Tower();
  /**
    * @brief checks if it can attack anyone in it's range, but first checks if there are entities that should be focused
    * @param map the map where it searches for entities
   */
  void attack(Map *map) override;
};
/**
  * @brief the class for the nexus, which doesn't do damage to entities, but if it dies, the game is over
  * and the team who destroyed it wins
 */
class Nexus : public Structure {
  public:
  /**
  * @brief set's up the nexuses attributes (base_hp,dmg)
   */
  Nexus();
};

/**
  * @brief a common class for camps
  * which are not able to move (baron nashor, drakes and jungle camps)
  *because of how the game works, every camp can give an effect to the champion(s) that slain it
 */
class Camp : public Entity {
public:
  /**
    * @brief set's up the camps attributes (base_hp,dmg)
   */
  Camp();
  /**
    * @brief set's the effect given by slaying this camp
    * @param e the effect to save
   */
  void setEffect(Effect e){effect = e;}
private:
  Effect effect;
};
/**
  * @brief the class that describes dragons,
  * there are different types of dragons, with different effects (currently only two)
 */
class Drake : public Camp{
public:
  /**
    * @brief set's up the dragons attributes (base_hp,dmg) and decides which type it should be
   */
  Drake();
  /**
    * @brief decides the type of effect that should be given by slaying this dragon
   */
  void decide_which_type();
};
/**
  * @brief class for minions,
  * which are a type of monsters that go through the lanes, attacking anything that's in front of them
 */
class Minion : public Entity {
public:
  /**
    * @brief set's up the minions attributes (base_hp,dmg)
    * @param side_ the team the minion is on
    * @param directions_ the vector of map positions, where the minion should go (top/mid/bot)
    * @param spawn_point the spawn_point of the minion on the minimap
   */
  Minion(Side side_, std::vector<sf::Vector2f> directions_, Cell *spawn_point);
  /**
    * @brief describes if the minion gives vision or not
   */
  bool gives_vision()const override{return true;}
  /**
    * @brief set's if minions should be focused
    * they should be focused by towers, so if a minion is under tower,
    * then it should attack the minion not other entities
   */
  bool should_focus()const override{return true;}
  /**
    * @brief describes if the minion increases creep score of the other entity that killed them or not
   */
  bool gives_creep_score()const override{return true;}
  /**
    * @brief does one move
    * first it checks if it can attack anything on the next cell it should go to, if not then moves to that cell
    * @param map the map where it should move on
   */
  void do_move(const std::shared_ptr<Map> &map);
  /**
    * @brief get's the next
   */
  sf::Vector2f get_next_direction_pos_index();
private:
  std::vector<sf::Vector2f> directions;
};
/**
  * @brief holds a wave of minions, and commands them
 */
class MinionWave{
public:
  /**
    * @brief initializes the size of minions in a minion wave
   */
  MinionWave():minion_wave_size(3){}
  /**
    * @brief spawns minions
    * @param startpoint the point where the minions first appear
    * @param directions_ the directions where the minions should try to move in
    * @param map the map where they move
    * @param side_ the side on which the minions are
   */
  void spawn(sf::Vector2f startpoint,const std::vector<sf::Vector2f>& directions_, const std::shared_ptr<Map>& map, Side side_);
  /**
    * @brief after the round ended, prepares for the next round
   */
  void round_end();
  /**
    * @brief does one move with the minions
    * @param map the map it moves on
   */
  void do_move(const std::shared_ptr<Map> &map);
private:
  std::list<Minion *> minions;
  std::vector<sf::Vector2f> directions;
  sf::Vector2f current_direction;
  size_t minion_wave_size;
};
/**
  * @brief the class that holds everything a player has
 */
class Player {
public:
  /**
    * @brief constructor for player class, set's it's properties to the appropriate values
    * @param champs the champions owned by this player
   */
  explicit Player(std::vector<Champion*> champs);
  /**
    * @brief frees heap allocated properties, which are the minion waves and champions
   */
  ~Player();
  /**
    * @brief spawns the champions on the map, informs the map about it, and set's the champs cell
    * @param map the map where they spawn
   */
  void spawn_champs(const std::shared_ptr<Map> &map);
  /**
    * @brief set's the champions spawn point, their base
    * @param point the spawnpoint
   */
  void set_spawn_point(Cell *spawn_point_);
  /**
    * @brief set's the champ icons, in order with each character of the string
    * @param icons the string to get the characters from
   */
  void set_champ_icons(const std::string &icons);
  /**
    * @brief set's all the champion's icons font face
    * @param holder the object that can be used to retrieve the font face
   */
  void set_font(Resources::Holder& holder);
  /**
    * @brief check's if there's a gamemove currently active on any of the champion
   */
  [[nodiscard]] bool is_gamemove_active() const;
  /**
    * @brief set's gamemove active to the given value
    * @param active true if there's a gamemove active, false if not
   */
  void set_gamemove_active(bool active);
  /**
    * @brief does the moves on each of the champions
    * @param map the map to do the gamemoves on
   */
  void do_moves(const std::shared_ptr<Map>& map);
  /**
    * @brief returns true, if the given champ is his
    * @param c the champ to check
   */
  bool is_his_champ(Champion *c);
  /**
    * @brief returns true, if this player started the game
   */
  [[nodiscard]] bool did_start()const{return starter;}
  /**
    * @brief set's if this player started the game, so it had the first turn
    * @param starter true if this player was the starter, otherwise false
   */
  void set_starter(bool starter_){starter = starter_;}
  /**
    * @brief check's if the round ended, returns true if it did
    * goes through the champions, and checks if they have movepoints left or not
   */
  bool check_round_end();
  /**
    * @brief spawns minions on to the map
    * @param map the map to spawn to
   */
  void spawn_minions(const std::shared_ptr<Map>& map);
  /**
    * @brief ends the round, calls the champions round end methods
    * @param map checks if minions should spawn, so it spawns them to the map
   */
  void round_end(std::shared_ptr<Map> &map);
  /**
    * @brief set's the simulation state for the player
    * @param sim if it's simulation then sim is true
   */
  void set_simulation(bool sim);
  /**
    * @brief update all of its champion position to the appropriate positions on the map
    * @param map the map is needed to determine where a given cell is
   */
  void update_champ_positions(std::shared_ptr<Map> map);
  /**
    * @brief set's the current players side
    * @param side the team where the player is on
   */
  void set_side(Side side);
  /**
    * @brief returns the players team side
   */
  [[nodiscard]] Side get_side()const{return side;}
  /**
    * @brief gets the selected champions on the given map index
   */
  Champion *get_selected_champs(sf::Vector2f index);
  /**
    * @brief set's the spawn point where the champions should spawn
   */
  [[nodiscard]] Cell *get_spawn_point()const{return spawn_point;}
  /**
    * @brief clears all the gamemoves from the champions
   */
  void clear_gamemoves();
  /**
    * @brief despawns the champions from the map
    * @param map the map where they should be removed from
   */
  void despawn_champs(std::shared_ptr<Map> &map);


private:
  std::vector<Champion*> champs;
  std::list<MinionWave> minion_waves;

  Side side;
  Cell *spawn_point;

  // save global buffs here, so the minions know, if there's nashor buff
  int minion_timer;
  int minion_timer_mark;
  bool is_simulation;
  bool gamemoveactive;
  bool starter;
};
#endif
