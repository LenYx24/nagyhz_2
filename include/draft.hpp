#ifndef DRAFT_HPP
#define DRAFT_HPP
#include "UIcomponents.hpp"
#include "game.hpp"
#include "gameobjects.hpp"
#include "ioparser.h"
#include "resources.hpp"
#include "statemanagement.hpp"
#include <fstream>
#include <vector>
/**
 * @brief class used to store one draft turn
 */
class DraftTurn {
public:
  /**
    * @brief constructor, initializes it's champs and if its ban_phase or not
    * @param champs the champs vector which should be used for it
   */
  explicit DraftTurn(std::vector<Champion *> &champs,bool ban_phase_ = false) : champs(champs), ban_phase(ban_phase_) {}
  /**
    * @brief does one turn
   */
  void do_turn(Champion *champ);
  /**
    * @brief returns the ban_phase variable, true if this turn is ban_phase
   */
  [[nodiscard]] bool is_ban_phase()const{return ban_phase;}
private:
  std::vector<Champion *> &champs;
  bool ban_phase;
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
  explicit DraftNamedBox(Resources::Holder &holder, sf::Vector2f size = {100,30});
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
  TeamCol(Resources::Holder &holder,sf::Vector2f start_pos_,sf::Vector2f size = {100,30}, float margin = 10);
  /**
    * @brief sets the position of the team column
   */
  void set_position();
  /**
    * @brief draws the teamcol to the window
   */
  void draw_to_window(sf::RenderWindow &window);
  /**
    * @brief gets the champions list size
   */
  [[nodiscard]] size_t champs_size()const{return champs.size();}
  /**
    * @brief gets the champion at the given index
    * @param index the champ at this index
   */
  Champion *operator[](size_t index){
    return champs[index];
  }
  /**
    * @brief returns the champ list
   */
  std::vector<Champion *> &get_champs(){return champs;}

protected:
  std::vector<DraftNamedBox> elements;
  std::vector<Champion *> champs;
  sf::Vector2f start_pos;
  float margin;
};
/**
  * @brief class that specializes Button, to a draftbutton with the correct design
 */
class DraftButton : public UI::Button {
public:
  DraftButton(
      Resources::Holder &h, const sf::String& str, [[maybe_unused]] std::function<void()> onclick = []() { std::cout << "not impl" << std::endl; });
};
/**
 * @brief a champion box implementation, which holds a champ
 */
class ChampBox : public UI::NamedBox {
public:
  ChampBox(const std::string& label, [[maybe_unused]]sf::RectangleShape frame, Resources::Holder &h, Champion *c)
      : NamedBox(label, std::move(frame), h), champ(c) {}
  /**
   * @brief gets the champion which is held in this box
   * @return the champion
   */
  Champion *get_champ()const{return champ;}
private:
  Champion *champ;
};
class DraftState : public State {
public:
  DraftState(StateManager &state_manager, Settings& settings, sf::RenderWindow& window);
  ~DraftState() override;
  void handle_events(sf::Event &event)override;
  void update()override;
  void draw(sf::RenderWindow& window)override;
  /**
   * @brief locks in the currently selected champion to the correct draftstate
   * @param state_manager
   * @param window
   * @param settings
   */
  void lock_in(StateManager& state_manager, sf::RenderWindow& window, Settings& settings);
  /**
   * @brief sets the currently selected champbox to an empty champion, which means the player didn't ban
   */
  void dont_ban();

protected:
  Resources::Holder holder;
  std::vector<Champion *> all_champs;
  Champion *selected_champ;
  std::vector<TeamCol> columns;
  sf::Clock elapsed_time;
  std::vector<DraftButton *> buttons;
  std::vector<ChampBox*> champ_list;
  Champion *empty_champ;
  std::vector<DraftTurn> turns;
  size_t turn_counter;
  sf::Text timer;
};

#endif
