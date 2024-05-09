#ifndef MAP_HPP
#define MAP_HPP
#include "gameobjects.hpp"
#include <vector>
#include <memory>

// specifies who is able to see the others monsters on this field
enum class vision { PLAYER1, PLAYER2, BOTH };
class Cell {
public:
  Cell():selected(false){}
  // updates the vision of the cell, should be called after every move
  virtual void updateVision();
  // returns true, if the 
  virtual bool canbuyitems();
  virtual void setselected();
  virtual bool canmovehere()const{return true;};
  virtual void addentity(Entity *entity);
  void setcolor(sf::Color c);
  bool contains(const int, const int);
  Entity *getentitiyclicked(const int, const int);
  // sets the current cell to a highlighted color, to indicate it's clickable
  virtual void sethighlighted();
  virtual void resetcolor();
  void setpos(sf::Vector2f pos);
  virtual void draw(sf::RenderWindow& window);
  virtual void updateshape(sf::Vector2f mappos, sf::Vector2f cellsize, float margin=2);
  inline sf::Vector2f getindex()const{return indicies;}
 
private:
  std::vector<Entity *> entities;
  sf::Color color;
  sf::Vector2f pos;
  sf::Vector2f indicies; // basically the index + 1 coordinates of the cell, it's useful if we want to calculate things
  sf::RectangleShape shape;
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
  bool canmovehere()const{return false};
};
// calculates vision differently than the ground object
class Bush : public Ground {
public:
  Bush();
};
// players with the correct side can buy items here
class SpawnArea : public Ground {
public:
  SpawnArea();
  bool canbuyitems()const{return true;}
};
// Todo: make map zoomable
class Map {
public:
  Map(sf::Vector2f pos);
  // frees every cell on the map
  ~Map();
  // tells every cell to draw its contents to the screen
  void draw(sf::RenderWindow& w);
  // checks on every cell, if there are certain entities that can interact
  void update();
  void spawn(Entity *entity, sf::Vector2f pos);
  Cell *getclickedcell(const int, const int);
  // given the position of a cell, it gives the nearby cells to it in a square, the square size depends on the distance given
  std::vector<Cell *> getnearbycells(sf::Vector2f pos, int distance = 1);
  // this is the amount of cells
  sf::Vector2f getcellgridsize()const{return size;}
  Champion *getselectedchamp();
  Cell *getcell(sf::Vector2f pos){
    posindex p_index = toposindex(pos);
    return cells[p_index.i][p_index.j];}
  void setselectednearbycells(Champion *c);
  bool inboundsrow(int p){return 0 <= p && p < size.x;}
  bool inboundscol(int p){return 0 <= p && p < size.y;}
  void resetcolors();


private:
  struct posindex{
    size_t i, j;
  };
  posindex toposindex(sf::Vector2f pos);
  sf::Vector2f position;
  sf::Vector2f size = {20,20};
  sf::Vector2f cellsize = {30,30};
// todo, use std array
  Cell ***cells;
};

#endif
