#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "UIcomponents.hpp"
#include "gameobjects.hpp"
#include "resources.hpp"
#include "statemanagement.hpp"
#include "game.hpp"
#include <vector>
/**
 * @brief class used to store one draft turn
 */
class DraftTurn {
public:
  /**
    * @brief constructor, initializes it's champs and if its banphase or not
    * @param champs the champs vector which should be used for it
   */
  DraftTurn(std::vector<Champion *> &champs,bool banphase = false) : champs(champs),banphase(banphase) {}
  /**
    * @brief does one turn
   */
  void doturn(Champion *c);
  /**
    * @brief returns the banphase variable, true if this turn is banphase
   */
  bool isbanphase()const{return banphase;}
private:
  std::vector<Champion *> &champs;
  bool banphase;
};
/**
  * @brief class that specializes NamedBox, to a NamedBox with the correct design
 */
class DraftNamedBox : public UI::NamedBox {
public:
  /**
    * @brief constructs the draftnamedbox with the correct design
    * @param holder the object that can get the font face for the component
    * @param size the size of the namedbox
   */
  explicit DraftNamedBox(Resources::Holder &holder, sf::Vector2f size = {100,30}):NamedBox(holder) {
    frame.setSize(size);
    frame.setFillColor(sf::Color::Red);
    label.setString("");
  }
};
/**
  * class that holds a column of champions
 */
class TeamCol {
public:
  /**
    * @brief constructs the draftnamedbox with the correct design
    * @param holder the object that can get the font face for the component
    * @param size the size of the teamcol
    * @param margin the margin between the elements
   */
  TeamCol(Resources::Holder &holder,sf::Vector2f startpos,sf::Vector2f size = {100,30}, float margin = 10);
  /**
    * @brief sets the position of the team column
   */
  void setpos();
  /**
    * @brief draws the teamcol to the window
   */
  void draw_to_window(sf::RenderWindow &w);
  /**
    * @brief gets the champions list size
   */
  size_t champs_size()const{return champs.size();}
  /**
    * @brief gets the champion at the given index, throws error if index is out of range
    * @param index the champ at this index
   */
  Champion *operator[](size_t index){
    if(index >= champs.size())throw std::out_of_range("index out of range inside team column");
    return champs[index];
  }
  /**
    * @brief returns the champ list
   */
  std::vector<Champion *> &get_champs(){return champs;}

protected:
  std::vector<DraftNamedBox> elements;
  std::vector<Champion *> champs;
  sf::Vector2f startpos;
  float margin;
};
/**
  * @brief class that specializes Button, to a draftbutton with the correct design
 */
class DraftButton : public UI::Button {
public:
  DraftButton(
      Resources::Holder &h, const sf::String& str, std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; });
};
/**
    * @brief champbox implementation
 */
class ChampBox : public UI::NamedBox {
public:
  ChampBox(std::string label, sf::RectangleShape frame, Resources::Holder &h, Champion *c) : NamedBox(label, frame, h), champ(c) {}
  Champion *get_champ()const{return champ;}
private:
  Champion *champ;
};
class DraftState : public State {
public:
  DraftState(StateManager &state_manager, const Settings& s, sf::RenderWindow& window);
  ~DraftState();
  void handle_events(sf::Event &e);
  void update();
  void draw(sf::RenderWindow& window);

  void lockin(StateManager& s, sf::RenderWindow& window, Settings settings);
  void dont_ban();

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
