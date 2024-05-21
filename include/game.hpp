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

class ItemBox : public UI::NamedBox {
public:
  ItemBox(const std::string& label, sf::RectangleShape frame, Resources::Holder &h, Item *i)
      : NamedBox(label, std::move(frame), h), item(i) {}
  Item *get_item()const{return item;}
private:
  Item *item;
};
class GameButton : public UI::Button {
public:
// Todo: static fgv, ami visszaad egy draftbutton stílusú gombot
  GameButton(
      Resources::Holder &h, const sf::String& str, std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; }, sf::Vector2f pos = {0,0});
};
class GameState : public State {
public:
  GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, Settings settings, sf::RenderWindow& window);
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
  GameMode mode;
  Resources::Holder h;
  // game elements
  std::vector<Item> allitems;
  std::vector<Player *> players;
  Player *currentplayer;
  // selection
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
};
#endif
