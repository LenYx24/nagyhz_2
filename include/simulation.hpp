#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <vector>
class SimulationState : public State {
  public:
  SimulationState(std::vector<Player *> &players, std::shared_ptr<Map> map, sf::RenderWindow& window, GameMode mode, StateManager &state_manager);
  ~SimulationState() {}
  virtual void handle_events(sf::Event &e);
  virtual void update();
  virtual void draw(sf::RenderWindow& window);

protected:
  Resources::Holder h;
  std::vector<Player *> &players;
  std::shared_ptr<Map> map;
  //std::vector<UI::Button> buttons;
  sf::Clock elapsed_time;
  sf::Time oneturn_time; // the amount of time the current turn that was done is shown
  UI::NamedBox *title;
  GameMode mode;
  int round_count;
  int round_counter;
  // timer
  sf::Text timer;
};
#endif
