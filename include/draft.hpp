#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "ioparser.hpp"
#include "statemanagement.hpp"
#include <list>
#include <vector>
class DraftTurn {
public:
  DraftTurn() {}

private:
  Player *currentplayer;
};
class DraftState : public State {
public:
  DraftState(const GameMode mode);
  ~DraftState() {}
  void HandleEvents(StateManager &s, Renderer &renderer);
  void Update(StateManager &s, Renderer &r);

protected:
  std::vector<Champion> allchamps;
  std::vector<Champion *> selectedchamp;
  std::vector<Champion *> bannedchamps;
  std::vector<Champion *> player1champs;
  std::vector<Champion *> player2champs;
  sf::Time elapsedtime;
  Button buttons[3];
  std::vector<DraftTurn> turns;
};

#endif
