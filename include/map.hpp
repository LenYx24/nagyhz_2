#ifndef MAP_HPP
#define MAP_HPP
#include "gameobjects.hpp"
#include <vector>
#include <memory>
enum class Side;
// specifies who is able to see the others monsters on this field
enum class vision { PLAYER1, PLAYER2, BOTH };
class Cell {
public:
  Cell():selected(false){}
  virtual ~Cell();
  // updates the vision of the cell, should be called after every move
  virtual bool should_update_vision_around(Side side_);
  // returns true, if the 
  virtual bool canbuyitems()const{return shop;}
  virtual void set_shop(bool shop_){shop=shop_;}
  virtual void setselected();
  virtual inline bool is_selected()const{return selected;}
  virtual bool canmovehere()const{return true;};
  virtual bool can_ward_here()const{return true;}
  virtual bool can_attack_entity()const{return entities.size() > 0;}
  virtual void addentity(Entity *entity);
  virtual void remove_entity(Entity *entity);
  void setcolor(sf::Color c);
  bool contains(const int, const int);
  Entity *getentitiyclicked(const int, const int);
  // sets the current cell to a highlighted color, to indicate it's clickable
  virtual void sethighlighted();
  virtual void resetcolor();
  virtual void set_vision(bool has_vision);
  void setpos(sf::Vector2f pos);
  virtual void draw(sf::RenderWindow& window);
  virtual void updateshape(sf::Vector2f mappos, sf::Vector2f cellsize, float margin=2);
  sf::Vector2f getindex()const{return indicies;}
  sf::Vector2f get_position()const{return pos;}
  void update_entities_shape(sf::Vector2f mappos);
  Entity *get_first_entity(){if(entities.size() == 0)return nullptr; else return entities[0];}
  Entity *get_attackable_entity(Side side_);
  void unselect();
 
private:
  std::vector<Entity *> entities;
  sf::Color color;
  sf::Vector2f pos;
  sf::Vector2f indicies; // basically the index + 1 coordinates of the cell, it's useful if we want to calculate things
  sf::RectangleShape shape;
  bool shop;
  bool selected;
};
// the basic cell type, that can be moved on by the player
class Ground : public Cell {
public:
  Ground();
};
// the only difference is that it has another color, but in the future there could be more unique things to it
class River : public Ground {
public:
  River();
};
// can't be moved on to by the player
class Wall : public Cell {
public:
  Wall();
  bool canmovehere()const{return false;};
};
// calculates vision differently than the ground object
class Bush : public Ground {
public:
  Bush();
};
class SpawnArea : public Ground {
public:
  SpawnArea();
};
class Map {
public:
  Map(sf::Vector2f pos);
  // frees every cell on the map
  ~Map();
  // tells every cell to draw its contents to the screen
  void draw(sf::RenderWindow& w);
  // called when you need the real position at the turn of the entities
  void update();
  void spawn(Entity *entity, sf::Vector2f pos);
  void despawn(Entity *entity, sf::Vector2f pos);
  Cell *getclickedcell(const int, const int);
  // given the position of a cell, it gives the nearby cells to it in a square, the square size depends on the distance given
  std::vector<Cell *> getnearbycells(sf::Vector2f pos, int distance = 1);
  // this is the amount of cells
  sf::Vector2u getcellgridsize()const{return size;}
  Champion *getselectedchamp();
  Cell *getcell(sf::Vector2f pos){
    posindex p_index = toposindex(pos);
    return cells[p_index.i][p_index.j];}
  template<typename P>
  void setselectednearbycells(Champion *c, P pred);
  void select_accessible_cells(Champion *c);
  void select_attackable_entities(Champion *c);
  void select_wardable_cells(Champion *c);
  bool inboundsrow(int p){return 0 <= p && p < static_cast<int>(size.x);}
  bool inboundscol(int p){return 0 <= p && p < static_cast<int>(size.x);}
  void move(Entity *entity, sf::Vector2f from, sf::Vector2f to);
  void reset_cell_selections();
  void update_vision();
  void update_vision_side(Side side_){vision_side = side_;}
  void check_game_end();
  bool did_game_end(){check_game_end();return game_end;}

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
// todo, use std array
  Cell *cells[20][20];
};

#endif
