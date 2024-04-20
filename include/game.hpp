#ifndef GAME_HPP
#define GAME_HPP
#include "gameobjects.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"

class Turn {
  // a turn starts for the player given in the arguments
  // the round variable is increased by one, the elapsed_time is reset to zero
  void start_turn(Player *p);
  // when the turn ends the object calls the currently pointed player to do its selected moves
  void end_turn();

private:
  sf::Uint32 round;
  sf::Uint32 points;
  const Player *player;
  sf::Time elapsed_time; // starts from zero when the turn starts
  sf::Time turn_time;
};
class GameState : public State {
protected:
  Resources::Holder h;
  Champion champs[10];
  Item *allitems;
};
#endif
