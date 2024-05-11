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
public:
  virtual bool is_complete(){return true;}
  virtual void finish(Cell *cell){}
  virtual Cell *position_cell(){return nullptr;}
  int get_movepoints()const{return points;}
protected:
  int points; // the amount of points needed to do the move
};

class MoveCell : public GameMove {
public:
  // the amount of cells, and the ones chosen to move
  MoveCell():cell(nullptr){}
  void setcell(Cell *cell){this->cell = cell;}
  bool is_complete(){return cell != nullptr;}
  void finish(Cell *cell){this->cell = cell;}
  virtual Cell *position_cell(){return cell;}
private:
  Cell *cell; // the cell to move to
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
