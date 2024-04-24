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
  DraftTurn(std::list<Champion *> &champs) : champs(champs) {}
  void doturn(Champion *c);

private:
  std::list<Champion *> &champs;
};

class DraftNamedBox : public UI::NamedBox {
public:
  DraftNamedBox(std::string) {
    _frame.setSize({100, 30});
    _frame.setFillColor(sf::Color::Red);
    _label.setString("Test");
  }
};
class TeamCol {
public:
  TeamCol(sf::Vector2f startpos, int margin = 10);
  void setpos();
  std::list<Champion *> champs;
  void draw_to_window(sf::RenderWindow &w);

protected:
  std::vector<DraftNamedBox> elements;
  sf::Vector2f startpos;
  int margin;
};
class DraftButton : public UI::Button {
public:
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
