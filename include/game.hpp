#ifndef GAME_HPP
#define GAME_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "map.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include "simulation.hpp"
#include <cstdlib>
#include <ctime>
#include <vector>

class ItemBox : public UI::NamedBox {
public:
  ItemBox(std::string label, sf::RectangleShape frame, Resources::Holder &h, Item *i) : NamedBox(label, frame, h), item(i) {}
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
  ~GameState();
  void handle_events(sf::Event &e);
  void update();
  void draw(sf::RenderWindow& window);

  void onclick_gamemove();
  void onclick_attack();
  void onclick_base();
  void onclick_ward();
  void onclick_item(Item *selected_item);

  bool is_gamemove_finisher(Cell *clickedcell);
  void end_turn();

  void show_cellinfo(sf::Vector2f index);
  void show_stats(std::vector<std::string> &statsentity);

  std::function<void()> create_simulation;
  void next_player();

private:
  GameMode mode;
  Resources::Holder h;
  // game elements
  std::vector<Item> allitems;
  std::vector<Player*> players;
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
