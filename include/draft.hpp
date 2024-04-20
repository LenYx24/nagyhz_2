#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "gameobjects.hpp"
#include "statemanagement.hpp"
#include <list>
class DraftState : public State {
public:
  DraftState(const GameMode mode);
  void HandleEvents(StateManager &s, Renderer &renderer);
  void Update(StateManager &s, Renderer &r);

protected:
  std::list<Champion> allchamps;
  std::list<Champion *> selectedchamp;
  std::list<Champion *> bannedchamps;
  std::list<Champion *> player1champs;
  std::list<Champion *> player2champs;
  sf::Time elapsedtime;
};
class DraftTurn {
  DraftTurn() {
    //
    banround = 1;
  }

private:
  int banround;         // goes from 1 to 6
  int banroundcount[2]; // , cuz there are 3 ban rounds
  bool banroundsdone;   // set to true, when ban phase is over
  Player *currentplayer;
};
#endif
