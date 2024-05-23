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
/**
 * @brief abstract class that is the base for all gamemoves
 */
class GameMove {
public:
  /**
    * @brief constructor that sets the gamemoves cell to a nullptr
   */
  GameMove(): cell(nullptr) {}
  /**
    * @brief the default destructor, doesn't need to free the cell, as its not his responsibility
   */
  virtual ~GameMove() = default;
  /**
    * @brief checks if the gamemove is complete, returns true if it is
   */
  [[nodiscard]] bool is_complete() const{return cell != nullptr;}
  /**
    * @brief gets the current position cell of the gamemove
   */
  [[nodiscard]] virtual Cell *position_cell() const{return cell;}
  /**
    * @brief finishes the gamemove, by giving it the cell to use
   */
  virtual void finish(Cell *cell_) {cell = cell_;}
  /**
   * the amount of points needed to perform this action
   * @return the points
   */
  [[nodiscard]] int get_movepoints()const{return points;}
  /**
   * sets the points
   * @param points_ the new points value
   */
  void set_movepoints(int points_){points=points_;}
  /**
    * @brief does the move with the champ on the map
    * @param champ the champ whose move it is
    * @param map the map to do the moves on
   */
  virtual void do_move(Champion *champ, std::shared_ptr<Map> map) = 0;
  /**
    * @brief checks if the gamemove is addable or not to the selected champion
    * @param current_player the currently selected player
    * @param selected_champ the currently selected champion
   */
  virtual bool check_gamemove_addable(Player *current_player, Champion *selected_champ);
  /**
   * @brief checks if this move changes entities position
   * @return true if this gamemove changes the entities position
   */
  [[nodiscard]] virtual bool changes_pos()const{return false;}
  /**
   * @brief gets this gamemoves state information
   * @return
   */
  virtual std::string get_state_info()const;
  /**
   * @brief returns a specialized standard formatted output string
   * @param name the name of the derived gamemoe
   * @return
   */
  std::string get_formatted_info(const std::string& name)const;

protected:
  int points = 1;
  Cell *cell; // the cell to move to
};

class MoveCell : public GameMove {
public:
  void do_move(Champion *champ, std::shared_ptr<Map> map)override;
  std::string get_state_info()const override;
  [[nodiscard]] bool changes_pos()const override{return true;}
};
/**
  * @brief the class that implements the attack move
 */
class AttackMove : public GameMove {
public:
  AttackMove(){
    set_movepoints(2);
  }
  [[nodiscard]] std::string get_state_info()const override;
  void do_move(Champion *champ, std::shared_ptr<Map> map) override;
};
/**
  * @brief the class that implements the ward placing mechanism
 */
class PlaceWard : public GameMove {
public:
  PlaceWard(){
    set_movepoints(2);
  }
  [[nodiscard]] std::string get_state_info()const override;
  void do_move(Champion *champ, std::shared_ptr<Map> map) override;
};
/**
  * @brief the class that implements the teleport to the base gamemove
 */
class TeleportBase : public GameMove {
public:
  TeleportBase(){
    set_movepoints(2);
  }
  [[nodiscard]] std::string get_state_info()const override;
  [[nodiscard]] bool changes_pos()const override{return true;}
  void do_move(Champion *champ, std::shared_ptr<Map> map)override;
};
#endif
