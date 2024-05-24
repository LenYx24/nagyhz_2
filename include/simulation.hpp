#ifndef SIMULATION_HPP
#define SIMULATION_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include "ioparser.h"
#include <vector>
/**
 * @brief the state that implements the simulation
 */
class SimulationState : public State {
  public:
    /**
     * @brief constructs the simulation state with the given params
     * @param players the players of the game
     * @param map the map where they play
     * @param window the window
     * @param settings the current settings of the game
     * @param state_manager the state_manager
     * @param callback_ the callback function to be called, when this state ends
     */
  SimulationState(std::vector<Player *> &players,
                    std::shared_ptr<Map> &map,
                    sf::RenderWindow& window,
                    Settings& settings,
                    StateManager &state_manager,
                    std::ofstream &output_file_,
                    std::function<void()> callback_ = [](){});
  ~SimulationState() override;
  void handle_events(sf::Event &event) override;
  void update() override;
  void draw(sf::RenderWindow& window) override;

private:
  Settings &settings;
  // the callback function that is run after simulation state ends
  std::function<void()>callback;
  Resources::Holder holder;
  std::vector<Player *> &players;
  std::shared_ptr<Map> map;
  //std::vector<UI::Button> buttons;
  sf::Clock elapsed_time;
  UI::NamedBox *title;
  int round_count = 3;
  int round_counter = 0;
  // timer
  sf::Text timer;
  // output
  std::ofstream &output_file;
};
#endif
