#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <list>
#include <vector>
class DraftTurn {
public:
  DraftTurn(std::vector<Champion *> &champs) : champs(champs) {}
  void doturn(Champion *c);

private:
  std::vector<Champion *> &champs;
};

class DraftNamedBox : public UI::NamedBox {
public:
  DraftNamedBox(Resources::Holder &h, std::string):NamedBox(h) {
    _frame.setSize({100, 30});
    _frame.setFillColor(sf::Color::Red);
    _label.setString("Test");
// doesnt show its text
  }
};
class TeamCol {
public:
  TeamCol(Resources::Holder &h,sf::Vector2f startpos,int margin = 10);
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
  void Draw(sf::RenderWindow &window);
  void lockin(StateManager& s);

protected:
  Resources::Holder h;
  std::vector<Champion> allchamps;
  Champion *selectedchamp;
  std::vector<TeamCol> columns;
  sf::Clock elapsedtime;
  std::vector<DraftButton *> buttons;
  std::vector<ChampBox *> champlist;
  std::vector<DraftTurn> turns;
  size_t turn_counter;
  sf::Text timer;
};

#endif
