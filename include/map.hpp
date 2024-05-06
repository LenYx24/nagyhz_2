#ifndef MAP_HPP
#define MAP_HPP
#include "gameobjects.hpp"
#include <vector>
// specifies who is able to see the others monsters on this field
enum class vision { PLAYER1, PLAYER2, BOTH };
class Cell {
public:
  // updates the vision of the cell, should be called after every move
  bool updateVision();
  // returns true, if the 
  virtual void canbuyitems() = 0;
  virtual bool canmovehere(){return canmove;}
  // sets the current cell to a highlighted color, to indicate it's clickable
  virtual void sethighlighted() = 0;
  void setpos(sf::Vector2f pos);

protected:
  std::vector<Entity> entities;
  bool canmove;
  sf::Color color;
  sf::Vector2f position; // basically the index + 1 coordinates of the cell, it's useful if we want to calculate things
};
// the basic cell type, that can be moved on by the player
class Ground : public Cell {
public:
  Ground();
  bool updateVision();
  virtual void canbuyitems();
  virtual void sethighlighted();
};
// the only difference is that it has another color, but in the future there could be more unique things to it
class River : public Ground {};
// can't be moved on to by the player
class Wall : public Cell {};
// calculates vision differently than the ground object
class Bush : public Ground {};
// players with the correct side can buy items here
class BlueSpawnArea : public Ground {};
class RedSpawnArea : public Ground {};
// Todo: make map zoomable
class Map {
public:
  Map();
  // frees every cell on the map
  ~Map();
  // tells every cell to draw its contents to the screen
  void draw();
  // checks on every cell, if there are certain entities that can interact
  void update();

  // given the position of a cell, it gives the nearby cells to it in a square, the square size depends on the distance given
  Cell *getnearbycells(sf::Vector2f pos, int distance = 1);

protected:
sf::Vector2f size = {20,20};
  Cell ***cells;
};

#endif
