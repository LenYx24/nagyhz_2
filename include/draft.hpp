#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include "game.hpp"
#include <list>
#include <vector>
class DraftTurn {
public:
  DraftTurn(std::vector<Champion *> &champs,bool banphase = false) : champs(champs),banphase(banphase) {}
  void doturn(Champion *c);
  bool isbanphase()const{return banphase;}
private:
  std::vector<Champion *> &champs;
  bool banphase;
};

class DraftNamedBox : public UI::NamedBox {
public:
  DraftNamedBox(Resources::Holder &h, std::string, sf::Vector2f size = {100,30}):NamedBox(h) {
    _frame.setSize(size);
    _frame.setFillColor(sf::Color::Red);
    _label.setString("");
  }
};
class TeamCol {
public:
  TeamCol(Resources::Holder &h,sf::Vector2f startpos,sf::Vector2f size = {100,30}, int margin = 10);
  void setpos();
  std::vector<Champion *> champs;
  void draw_to_window(sf::RenderWindow &w);

protected:
  std::vector<DraftNamedBox> elements;
  sf::Vector2f startpos;
  int margin;
};
class DraftButton : public UI::Button {
public:
// Todo: static fgv, ami visszaad egy draftbutton stílusú gombot
  DraftButton(
      Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick = [](StateManager &s) { std::cout << "not impl" << std::endl; });
};

class ChampBox : public UI::NamedBox {
public:
  ChampBox(std::string label, sf::RectangleShape frame, Resources::Holder &h, Champion *c) : NamedBox(label, frame, h), champ(c) {}
  Champion *champ;
};
class DraftState : public State {
public:
  DraftState(StateManager &state_manager, const Settings s);
  ~DraftState();
  void HandleEvents(sf::Event &e);
  void Update();
  void Draw();
  void lockin(StateManager& s);
  void dontban(StateManager& s);

protected:
  Resources::Holder h;
  std::vector<Champion *> allchamps;
  Champion *selectedchamp;
  std::vector<TeamCol> columns;
  sf::Clock elapsedtime;
  std::vector<DraftButton *> buttons;
  std::vector<ChampBox*> champlist;
  Champion *emptychamp;
  std::vector<DraftTurn> turns;
  size_t turn_counter;
  sf::Text timer;
};

#endif
