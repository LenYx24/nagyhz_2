#ifndef GAME_HPP
#define GAME_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <cstdlib>
#include <list>
#include <vector>

class Turn {
public:
  // a turn starts for the player given in the arguments
  // the round variable is increased by one, the elapsed_time is reset to zero
  void start_turn(Player *p);
  // when the turn ends the object calls the currently pointed player to do its selected moves
  void end_turn();
  // calls a sub state, that runs the simulation of the moves
  void simulate_game();

private:
  int points;
  const Player *player;
  sf::Time elapsed_time; // starts from 0
  sf::Time turn_time;    // something like 1 minute to do the moves
};
// responsible for doing one round
class Round {
public:
  // sets the variables necessary to start a round
  Round(std::vector<Player> players);
  // when the round ends the simulation substate starts
  void roundend();

private:
  Turn turns[2];
};
class GameState : public State {
  GameState(Champion *allchamps[10], GameMode mode);
  ~GameState() {}
  void HandleEvents(StateManager &s, Renderer &renderer);
  void Update(StateManager &s, Renderer &r);

  void nextplayer();
  Player *getcurrentplayer();

protected:
  Resources::Holder h;
  std::list<Item> allitems;
  std::vector<Player> players;
  Map *map;
  std::vector<GameMove> gamemoves;
  GameMove *selectedmove;
  std::vector<Round> rounds;
  std::vector<Button> buttons;
};
#endif
