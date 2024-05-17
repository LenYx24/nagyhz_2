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

// class Turn {
// public:
//   // a turn starts for the player given in the arguments
//   // the round variable is increased by one, the elapsed_time is reset to zero
//   void start_turn(Player *p);
//   // when the turn ends the object calls the currently pointed player to do its selected moves
//   void end_turn();

// private:
//   int points;
//   const Player *player;
//   sf::Time elapsed_time; // starts from 0
//   sf::Time turn_time;    // something like 1 minute to do the moves
// };
// // responsible for doing one round
// class Round {
// public:
//   // sets the variables necessary to start a round
//   Round();
//   // when the round ends the simulation substate starts
//   void roundend();

// private:
//   std::array<Turn, 2> turns;
//   int currentturn;
// };
class ItemBox : public UI::NamedBox {
public:
  ItemBox(std::string label, sf::RectangleShape frame, Resources::Holder &h, Item *i) : NamedBox(label, frame, h), item(i) {}
  Item *item;
};
class GameButton : public UI::Button {
public:
// Todo: static fgv, ami visszaad egy draftbutton stílusú gombot
  GameButton(
      Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick = [](StateManager &s) { std::cout << "not impl" << std::endl; }, sf::Vector2f pos = {0,0});
};
class GameState : public State {
public:
  GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, GameMode mode, sf::RenderWindow& window);
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
  // Player *getcurrentplayer();

private:
  GameMode mode;
  Resources::Holder h;
  // game elements
  std::vector<Item> allitems;
  std::vector<Player*> players;
  Player *currentplayer;
  //std::vector<Round *> rounds;
  std::shared_ptr<Map> map;
  // selection
  Champion *selectedchamp;
  // UI
  std::vector<ItemBox *> itemslist;
  std::vector<GameButton*> buttons;
  std::vector<GameButton*> gamemovebuttons;
  std::vector<UI::NamedBox*> labels;
  std::vector<UI::NamedBox*> statlabels;
  // Todo: make this into a class
  // timer
  sf::Text timer;
  sf::Clock elapsedtime;
};
#endif
