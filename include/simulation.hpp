#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "map.hpp"
#include "statemanagement.hpp"
#include <vector>
class SimulationState : public State {
  SimulationState(Champion *allchamps[10], GameMode mode);
  ~SimulationState() {}
  void HandleEvents(StateManager &s, Renderer &renderer);
  void Update(StateManager &s, Renderer &r);

  void doOneMove();

protected:
  Resources::Holder h;
  std::vector<Player> players;
  Map *map;
  std::vector<Button> buttons;
  sf::Time elapsed_time;
  sf::Time oneturn_time; // the amount of time the current turn that was done is shown
};
#endif
