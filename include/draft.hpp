#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "ioparser.hpp"
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

class DraftButton : public Button {
public:
  DraftButton(
      Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick = [](StateManager &s) { std::cout << "not impl" << std::endl; });
};
class DraftState : public State {
public:
  DraftState(const Settings s);
  ~DraftState() {}
  void HandleEvents(StateManager &s, Renderer &renderer);
  void Update(StateManager &s, Renderer &r);

protected:
  std::vector<Champion> allchamps;
  std::vector<Champion *> selectedchamp;
  std::vector<Champion *> p1banchamps;
  std::vector<Champion *> p2banchamps;
  std::vector<Champion *> p1champs;
  std::vector<Champion *> p2champs;
  sf::Time elapsedtime;
  std::vector<DraftButton> buttons;
  std::vector<DraftTurn> turns;
};

#endif
