#ifndef GAMEMOVES_HPP
#define GAMEMOVES_HPP
///
/// The standard class for a move in the game, each move has to be derived from this class
///
#include <SFML/Graphics.hpp>
class Cell;
class Champion;
class Entity;
class GameMove {
protected:
  int points; // the amount of points needed to do the move
};

class MoveCells : public GameMove {
public:
  // the amount of cells, and the ones chosen to move
  MoveCells(int amount, std::vector<Cell *> cells);
  // does one move, removes the first elemnent of cells, and checks if there's a fight
  void moveone(Cell *from, Cell *destination);
  void update_is_legal();

protected:
  std::vector<Cell *> cells; // the cells in order which the
  bool is_legal;
};
// this one could be a template class, or heterogen collection
class AttackMove : public GameMove {
public:
  AttackMove(int amount, std::vector<Entity *> entities);
  void removehp();
};
// removes the ward from the player, calls the appropriate function
class PlaceWard : public GameMove {};
// the champion cannot move, and if no one interrupts it, then it goes back to the base
class TeleportBase : public GameMove {};
#endif
