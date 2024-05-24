#ifndef GAME_HPP
#define GAME_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "ioparser.h"
#include "map.hpp"
#include "resources.hpp"
#include "simulation.hpp"
#include "statemanagement.hpp"

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <vector>
/**
 * @brief a specialized namedbox class that holds an item
 */
class ItemBox : public UI::NamedBox {
public:
  /**
   * @brief constructs an itembox
   * @param label the label on the box
   * @param frame the frame to put the box inside
   * @param holder the resource holder
   * @param item the item to put inside
   */
  ItemBox(const std::string& label, sf::RectangleShape frame, Resources::Holder &holder, Item *item);
  /**
   * @brief gets the held item
   * @return the item
   */
  Item *get_item()const{return item;}
private:
  Item *item;
};
/**
 * @brief a button that has a specific style used for game buttons
 */
class GameButton : public UI::Button {
public:
  /**
   * @brief constructs a gamebutton
   * @param holder the resources holder
   * @param str the title of the button
   * @param onclick the onclick that has to be called if the gamebutton gets clicked
   * @param pos the position of the gamebutton on the window
   */
  GameButton(
      Resources::Holder &holder, const sf::String& str, std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; }, sf::Vector2f pos = {0,0});
};
/**
 * @brief the state that is responsible for navigating through a game
 */
class GameState : public State {
public:
  GameState(StateManager &state_manager,
            std::vector<Champion*> p1champs,
            std::vector<Champion*> p2champs,
            Settings& settings,
            sf::RenderWindow& window);
  ~GameState() override;
  void handle_events(sf::Event &e)override;
  void update()override;
  void draw(sf::RenderWindow& window)override;

  void onclick_gamemove();
  void onclick_attack();
  void onclick_base();
  void onclick_ward();
  void onclick_item(Item *selected_item);
  void onclick_reset_gamemove();

  bool is_gamemove_finisher(Cell *clicked_cell);
  void end_turn();

  void show_cell_info(sf::Vector2f index);
  void show_stats(std::vector<std::string> &stats);

  std::function<void()> create_simulation;
  void next_player();

private:
  Resources::Holder h;
  Settings &settings;
  // game elements
  std::vector<Item> allitems;
  std::vector<Player *> players;
  // selection
  Player *currentplayer;
  Champion *selectedchamp;
  // UI
  int time_left = 60;
  std::vector<ItemBox *> items_boxes;
  std::vector<GameButton*> buttons;
  std::vector<GameButton*> gamemove_buttons;
  std::vector<UI::NamedBox*> labels;
  std::vector<UI::NamedBox*> stat_labels;
  // map
  std::shared_ptr<Map> map;
  // timer
  sf::Text timer;
  sf::Clock elapsed_time;
  // output
  std::ofstream output_file;
};
#endif
