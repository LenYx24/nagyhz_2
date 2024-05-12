#ifndef GAMEMOVES_HPP
#define GAMEMOVES_HPP
///
/// The standard class for a move in the game, each move has to be derived from this class
///
#include <SFML/Graphics.hpp>
#include <memory>
class Cell;
class Player;
class Champion;
class Map;
class Entity;
class GameMove {
public:
  GameMove() : points(1), cell(nullptr) {}
  virtual ~GameMove() {}
  void setcell(Cell *cell) {
    this->cell = cell;
  }
  bool is_complete() {
    return cell != nullptr;
  }
  virtual Cell *position_cell() {
    return cell;
  }
  virtual void finish(Cell *cell) {
    this->cell = cell;
  }
  // make this one abstract, after the testing is done
  virtual void do_move(Champion *champ, std::shared_ptr<Map> map){}
  virtual void onclick(std::shared_ptr<Map> map, Player *currentplayer, Champion *selected_champ) = 0;
  int get_movepoints() const {
    return points;
  }
  void set_movepoints(int p) {
    this->points = p;
  }
  bool basic_check(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ) const;

protected:
  int points; // the amount of points needed to do the move
  Cell *cell; // the cell to move to
};

class MoveCell : public GameMove {
public:
  // the amount of cells, and the ones chosen to move
  virtual void onclick(std::shared_ptr<Map> map, Player *currentplayer, Champion *selected_champ);
  void do_move(Champion *champ, std::shared_ptr<Map> map);

private:
};
// this one could be a template class, or heterogen collection
class AttackMove : public GameMove {
public:
  AttackMove() {
    set_movepoints(3);
  }
  virtual void onclick(std::shared_ptr<Map> map, Player *currentplayer, Champion *selected_champ);
  AttackMove(int amount, std::vector<Entity *> entities);
  void removehp();
  void do_move(Champion *champ, std::shared_ptr<Map> map);


private:
  Entity *attacked_entity;
};
// removes the ward from the player, calls the appropriate function
class PlaceWard : public GameMove {
public:
  virtual void onclick(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ);
  void do_move(Champion *champ, std::shared_ptr<Map> map);
};
// the champion cannot move, and if no one interrupts it, then it goes back to the base
class TeleportBase : public GameMove {
public:
  TeleportBase() {
    set_movepoints(3);
  }
  virtual void onclick(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ);
  void do_move(Champion *champ, std::shared_ptr<Map> map);
};
#endif
