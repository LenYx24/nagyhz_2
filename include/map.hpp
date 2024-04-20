#ifndef MAP_HPP
#define MAP_HPP
#include "gameobjects.hpp"
#include <vector>
// river is the same as grass, but has a different color
enum class fieldtype { GRASS, RIVER, WALL, BUSH, BLUESPAWNAREA, REDSPAWNAREA };
// specifies who is able to see the others monsters on this field
enum class vision { PLAYER1, PLAYER2, BOTH };
class Cell {
public:
  // updates the vision of the cell, should be called after every move
  bool updateVision();

protected:
  std::vector<Entity> entities;
  fieldtype type;
};
// Todo: make map zoomable
class Map {
  // tells every cell to draw its contents to the screen
  void draw();
  // checks on every cell, if there are certain entities that can interact
  void update();

protected:
  Cell cells[32][32];
};

#endif
