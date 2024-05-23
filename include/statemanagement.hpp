#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <stack>
#include <utility>

class State;
class StateManager {
public:
  StateManager():pop(false){buffer_state = std::unique_ptr<State>(nullptr);}
  void change_state(std::unique_ptr<State> state);
  void push_state(std::unique_ptr<State> state);
  void pop_state();
  void update_state();

  void handle_events(sf::RenderWindow &window);
  void update();
  void draw(sf::RenderWindow &window);

  static sf::Vector2f get_size(sf::RenderWindow& window);
  [[nodiscard]] inline bool has_state() const {return !states.empty() || buffer_state;}

  void exit();

private:
  std::stack<std::unique_ptr<State>> states;
  std::unique_ptr<State> buffer_state;
  bool pop;
};
class State {
public:
  virtual ~State() = default;
  virtual void handle_events(sf::Event &event) = 0;
  virtual void update() = 0;
  virtual void draw(sf::RenderWindow& window) = 0;

protected:
  explicit State(StateManager &state_manager) : state_manager(state_manager) {}
  StateManager &state_manager;
};
enum class GameMode { TWO_PLAYER };
class Settings {
public:
  Settings(std::string champs_filepath, std::string items_filepath,std::string output_prefix, GameMode mode);
  [[nodiscard]] std::string get_champs_filepath()const{return champs_filepath;};
  [[nodiscard]] std::string get_items_filepath()const{return items_filepath;};
  [[nodiscard]] std::string get_output_prefix()const{return output_prefix;};
  [[nodiscard]] GameMode get_mode()const{return mode;};
  void set_champs_filepath(std::string path){champs_filepath=path;}
  void set_items_filepath(std::string path){items_filepath=path;}
  void set_output_prefix(std::string path){output_prefix=path;}
  void set_gamemode(GameMode mode_){mode=mode_;}
private:
  std::string champs_filepath;
  std::string items_filepath;
  std::string output_prefix;
  GameMode mode;
};
#endif
