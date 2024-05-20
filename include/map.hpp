#ifndef MAP_HPP
#define MAP_HPP
#include "gameobjects.hpp"
#include <vector>
#include <filesystem>
#include <memory>
enum class Side;
/**
  * @brief the base class for a cell on the map
 */
class Cell {
public:
  /**
    * @brief cell constructor
   */
  Cell():selected(false){}
  /**
    * @brief deletes the owned entities from the map
   */
  virtual ~Cell();
  /**
    * @brief updates the vision of the cell, should be called after every move and round ends
    * @param side_ which side should have vision
   */
  virtual bool should_update_vision_around(Side side_);
  /**
    * @brief returns true if entities can buy items on this cell
   */
  virtual bool can_buy_items()const{return shop;}
  /**
    * @brief set's this cell property to a shop cell
    * @param shop true if this is a cell where entities can shop
   */
  virtual void set_shop(bool shop_){shop=shop_;}
  /**
    * @brief sets the current cell as selected
   */
  virtual void set_selected();
  /**
    * @brief returns true if the current cell is selected
   */
  virtual bool is_selected()const{return selected;}
  /**
    * @brief true if entities are able to move here
   */
  virtual bool can_move_here()const{return true;};
  /**
    * @brief true if champions are able to put wards on this spot
   */
  virtual bool can_ward_here()const{return true;}
  /**
    * @brief returns if there are entities to attack on this cell
    * @param enemy_side_ returns true only if there are entities that aren't from this side
   */
  virtual bool can_attack_entity(Side enemy_side_)const;
  /**
    * @brief adds an entity to its entity list
    * @param entity the entity to add
   */
  virtual void add_entity(Entity *entity);
  /**
    * @brief removes the entity from the entity list
    * @param entity the entity to remove
   */
  virtual void remove_entity(Entity *entity);
  /**
    * @brief sets the cell's color
    * @param color the color to set it to
   */
  void set_color(sf::Color color);
  /**
    * @brief checks if the given coordinates are inside the cell
    * @param x coordinate
    * @param y coordinate
   */
  bool contains(const int x, const int y);
  /**
    * @brief get's the entity clicked by the given coordinates
    * @param x coordinate
    * @param y coordinate
   */
  Entity *get_entity_clicked(const int x, const int y);
  /**
    * @brief sets the current cell to a highlighted color, to indicate it's clickable
   */
  virtual void set_highlighted();
  /**
    * @brief only resets cell color to the default one, if it isn't a vision cell
   */
  virtual void reset_selection_color();
  /**
    * @brief sets back cell to having vision, but doesn't change it if it's selected
   */
  virtual void reset_vision_color();
  /**
    * @brief sets the property vision to the given argument
    * @param vision true if this cell has vision
   */
  virtual void set_vision(bool has_vision_);
  /**
    * @brief sets the current cells position
    * @param pos the position to change to
   */
  void set_position(sf::Vector2f pos_);
  /**
    * @brief draws the cell and its entities to the screen
    * @param window window to draw to
   */
  virtual void draw(sf::RenderWindow& window);
  /**
    * @brief updates the shapes properties
    * @param map_position the map position where this shape should be
    * @param cell_size the size of this cell
    * @param margin the margin to leave between it's neighbours
   */
  virtual void update_shape(sf::Vector2f map_position, sf::Vector2f cell_size, float margin= 2);
  /**
    * @brief gets the current index, this is where the cell is on the map
   */
  sf::Vector2f get_index()const{return indicies;}
  /**
    * @brief updates the entities positions on the given map position
    * @param mappos where it should put the shapes
   */
  void update_entities_shape(sf::Vector2f mappos);
  /**
    * @brief gets the first entity on the cell
   */
  Entity *get_first_entity(){if(entities.empty())return nullptr; else return entities[0];}
  /**
    * @brief gets an entity that is attackable and is on the other side than given in params
    * @params side_ the side which the entity asking is on, so it gives an entity of the opposing team
   */
  Entity *get_attackable_entity(Side side_);
  /**
    * @brief unselects the current cell
   */
  void unselect();
 
private:
  std::vector<Entity *> entities;
  sf::Color color;
  sf::Vector2f pos;
  sf::Vector2f indicies; // basically the index + 1 coordinates of the cell, it's useful if we want to calculate things
  sf::RectangleShape shape;

  bool shop;
  bool selected;
  bool has_vision;

  static sf::Uint8 has_vision_opacity;
  static sf::Uint8 no_vision_opacity;
  static sf::Uint8 selected_opacity;
};
/**
  * @brief the basic cell type, that can be moved on by the player
 */
class Ground : public Cell {
public:
  Ground();
};
/**
  * @brief the only difference from ground is that it has another color
 */
class River : public Ground {
public:
  River();
};
/**
  * @brief can't be moved on to by entities
 */
class Wall : public Cell {
public:
  Wall();
  /**
    * @brief set's the cells property of moving here to false
   */
  bool can_move_here()const override{return false;};
};
/**
  * @brief calculates vision differently than the ground object
 */
class Bush : public Ground {
public:
  Bush();
};
/**
  * @brief spawn area, where champions spawn
 */
class SpawnArea : public Ground {
public:
  SpawnArea();
};
/**
  * @brief the class that describes the map
 */
class Map {
public:
  /**
    * @brief set's the map's default properties, and its position on the window
    * @param pos the position on the window
   */
  Map(sf::Vector2f pos);
  /**
    * @brief frees every cell on the map
   */
  ~Map();
  /**
    * @brief tells every cell to draw its contents to the screen
    * @param window where it draws its contents
   */
  void draw(sf::RenderWindow& window);
  /**
    * @brief spawns an entity on the given position to the map
    * @param entity the entity to spawn
    * @param pos the position where to spawn it
   */
  void spawn(Entity *entity, sf::Vector2f pos);
  /**
    * @brief despawns an entity on the given position to the map
    * @param entity the entity to de_spawn
    * @param pos the position where to de_spawn it from
   */
  void de_spawn(Entity *entity, sf::Vector2f pos);
  /**
    * @brief returns the cell clicked on
    * @param x coordinate
    * @param y coordinate
   */
  Cell *get_clicked_cell(const int x, const int y);
  /**
    * @brief gets the nearby cell pointers into a vector
    * @param pos this is the middle cell
    * @param distance this is how far away a cell should be to cound it as nearby
   */
  std::vector<Cell *> getnearbycells(sf::Vector2f pos, int distance = 1);
  /**
    * @brief gets the amount of cells that are on the map
   */
  [[nodiscard]] sf::Vector2u get_cell_grid_size()const{return size;}
  /**
    * @brief gets the currently selected champion
   */
  Champion *getselectedchamp();
  /**
    * @brief gets cell at the given position
    * @pos the position
   */
  Cell *getcell(sf::Vector2f pos){
    posindex p_index = toposindex(pos);
    return cells[p_index.i][p_index.j];}
  /**
    * @brief sets the selected nearby cells which satisfy the predicate function
    * @param champ, the champion which is in the middle
    * @param pred the predicate which should be satisfied by the cell
   */
  template<typename P>
  void setselectednearbycells(Champion *champ, P pred);
  /**
    * @brief selects accessible cells where the champion could move
    * @param champ the champion to use
   */
  void select_accessible_cells(Champion *champ);
  /**
    * @brief select attackable entites, which could be attacked by the champ
    * @param champ the champion to use
   */
  void select_attackable_entities(Champion *champ);
  /**
    * @brief select cells where the champion could ward
    * @param champ the champion to use
   */
  void select_wardable_cells(Champion *c);
  /**
    * @brief checks if the given number is a good index inside the map
    * @param p the x coordinate
   */
  bool inboundsrow(int p){return 0 <= p && p < static_cast<int>(size.x);}
  /**
    * @brief checks if the given number is a good index inside the map
    * @param p the y coordinate
   */
  bool inboundscol(int p){return 0 <= p && p < static_cast<int>(size.x);}
  /**
    * @brief moves the entity from one cell to another
    * @param entity the entity to move
    * @param from the index where to move it from
    * @param to the index where to move it into
   */
  void move(Entity *entity, sf::Vector2f from, sf::Vector2f to);
  /**
    * @brief resets every cells selection
   */
  void reset_cell_selections();
  /**
    * @brief updates the vision of the map
   */
  void update_vision();
  /**
    * @brief updates the vision for the appropriate side
    * @param side_ the side which has vision of the map
   */
  void update_vision_side(Side side_){vision_side = side_;}
  /**
    * @brief checks if the game has concluded
   */
  void check_game_end();
  /**
    * @brief returns true if the game end, false otherwise
   */
  bool did_game_end(){check_game_end();return game_end;}
  /**
   * @brief disables vision on the map
   */
  void disable_vision();
  /**
   * @brief resets all the cells vision to having vision, but doesnt change fields that are selected
   */
  void reset_cell_vision();

private:
  struct posindex{
    size_t i, j;
  };
  posindex toposindex(sf::Vector2f pos);
  bool game_end = false;
  Side vision_side; // the side which has vision
  sf::Vector2f position;
  sf::Vector2u size = {20,20};
  sf::Vector2f cellsize = {30,30};
  std::vector<Entity *> nexuses;
// Todo: use std array
  Cell *cells[20][20];
};

#endif
