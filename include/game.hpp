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
public:
  GameState(StateManager &state_manager, std::vector<Champion *> p1champs,std::vector<Champion *> p2champs, GameMode mode);
  ~GameState();
  void HandleEvents(sf::Event &e);
  void Update();
  void Draw(sf::RenderWindow &window);

  // void nextplayer();
  // Player *getcurrentplayer();

protected:
  GameMode _mode;
  Resources::Holder h;
  std::list<Item> allitems;
  std::vector<Player*> players;
  std::vector<GameMove> gamemoves;
  GameMove *selectedmove;
  std::vector<Round> rounds;
  // UI
  std::vector<UI::Button> buttons;
  std::unique_ptr<Map> map;
  // timer
  sf::Text timer;
  sf::Clock elapsedtime;
};
#endif
