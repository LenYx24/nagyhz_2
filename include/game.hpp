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
  /**
   * @brief constructs the gamestate
   * @param state_manager the state manager of the application
   * @param p1champs the champions of the first player
   * @param p2champs the champions of the second player
   * @param settings the application settings
   * @param window a reference to the window
   */
  GameState(StateManager &state_manager,
            std::vector<Champion*> p1champs,
            std::vector<Champion*> p2champs,
            Settings& settings,
            sf::RenderWindow& window);
  ~GameState() override;
  void handle_events(sf::Event &e)override;
  void update()override;
  void draw(sf::RenderWindow& window)override;

  /**
   * @brief handles onclick of the movecell gamemove button
   */
  void onclick_movecell();
  /**
   * @brief handles onclick of the attack gamemove button
   */
  void onclick_attack();
  /**
   * @brief handles onclick of the base gamemove button
   */
  void onclick_base();
  /**
   * @brief handles onclick of the ward gamemove button
   */
  void onclick_ward();
  /**
   * @brief handles onclick of the item box, if the champ can buy the item, then it does
   */
  void onclick_item(Item *selected_item);
  /**
   * @brief removes the last gamemove of the selected champion, if he has an unfinished one
   */
  void onclick_reset_gamemove();

  /**
   * @brief after a gamemove which does move the player, this should be called to update the surroundings and the player
   */
  void after_gamemove();

  /**
   * @brief checks if clicking on the given cell at the current state of the game finishes the last move of a champion
   * @param clicked_cell the cell clicked on
   * @return true if this action should finish a gamemove of a champ
   */
  bool is_gamemove_finisher(Cell *clicked_cell);
  /**
   * @brief ends the current player's turn
   */
  void end_turn();

  /**
   * @brief show's cell information at the given index
   * @param index
   */
  void show_cell_info(sf::Vector2f index);
  /**
   * @brief shows statistics on the window
   * @param stats the statistics to show
   */
  void show_stats(std::vector<std::string> &stats);
  /**
   * @brief sets the current_player to the next one
   */
  void next_player();

  /**
   * @brief should be called if a simulation state is needed
   */
  std::function<void()> create_simulation;

private:
  Resources::Holder h;
  Settings &settings;
  // game elements
  std::vector<Item> all_items;
  std::vector<Player *> players;
  // selection
  Player *current_player;
  Champion *selected_champ;
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
