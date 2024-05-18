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
  ~SimulationState() override;
  void handle_events(sf::Event &e) override;
  void update() override;
  void draw(sf::RenderWindow& window) override;

protected:
  Resources::Holder h;
  std::vector<Player *> &players;
  std::shared_ptr<Map> map;
  //std::vector<UI::Button> buttons;
  sf::Clock elapsed_time;
  UI::NamedBox *title;
  GameMode mode;
  int round_count;
  int round_counter;
  // timer
  sf::Text timer;
};
#endif
