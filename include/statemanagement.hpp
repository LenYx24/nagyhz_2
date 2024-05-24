#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <SFML/Graphics.hpp>
#include <memory>
#include <stack>
#include <utility>

class State;
/**
 * @brief the class that handles the state management for this application
 */
class StateManager {
public:
  /**
   * @brief default constructor for the state manager
   */
  StateManager():pop(false){buffer_state = std::unique_ptr<State>(nullptr);}
  /**
   * @brief buffers the given state, so at the end of the main loop it gets changed to this
   * That means the current state will get removed, and replaced by this one
   * @param state the state to change to
   */
  void change_state(std::unique_ptr<State> state);
  /**
   * @brief buffers the given state, so at the end of the main loop it gets on top of the other states
   * That means the already existing states stay intact
   * @param state
   */
  void push_state(std::unique_ptr<State> state);
  /**
   * @brief buffers a pop state event until the main loop has ended
   * This means the upmost state gets removed and the one under it becomes the current state
   */
  void pop_state();
  /**
   * @brief updates the internal states variable according to the buffer and the last commanded action
   */
  void update_state();

  /**
   * @brief handles the events of the application, and calls the appropriate method of the current state
   * @param window the window that gives the events
   */
  void handle_events(sf::RenderWindow &window)const;
  /**
   * @brief calls the current state's update method
   */
  void update();
  /**
   * @brief tells the current state to draw its contents to the window
   * @param window the window to draw to
   */
  void draw(sf::RenderWindow &window);

  /**
   * @brief gets the size of the window in sf::Vector2f type
   * @param window the window whose size will be calculated
   * @return the size of the window
   */
  static sf::Vector2f get_size(sf::RenderWindow& window);
  /**
   * checks if there are states
   * @return true if there are, false if not
   */
  [[nodiscard]] bool has_state() const {return !states.empty() || buffer_state;}

  /**
   * @brief tells all its states to exit
   */
  void exit();

private:
  std::stack<std::unique_ptr<State>> states;
  std::unique_ptr<State> buffer_state;
  bool pop;
};
/**
 * @brief the abstract State class which is used to handle one state
 */
class State {
public:
  virtual ~State() = default;
  /**
   * @brief handles the given event
   * @param event the event to be handled
   */
  virtual void handle_events(sf::Event &event) = 0;
  /**
   * @brief updates the states
   */
  virtual void update() = 0;
  /**
   * @brief draws the state's contents to the given window
   * @param window the window to draw to
   */
  virtual void draw(sf::RenderWindow& window) = 0;

protected:
  explicit State(StateManager &state_manager) : state_manager(state_manager) {}
  StateManager &state_manager;
};
/**
 * @brief the gamemode of the game
 */
enum class GameMode { TWO_PLAYER };
/**
 * @brief the settings class, which holds the applications settings that could be needed at any state
 */
class Settings {
public:
  /**
   * @brief constructs a simple settings object
   * @param champs_filepath the filepath to the champions textfile
   * @param items_filepath the filepath to the items textfile
   * @param output_prefix the prefix of the output textfile
   * @param mode the gamemode of the applicatino
   */
  Settings(std::string champs_filepath, std::string items_filepath,std::string output_prefix, GameMode mode);
  /**
   * @return the champions filepath
   */
  [[nodiscard]] std::string get_champs_filepath()const{return champs_filepath;};
  /**
   * @return the items filepath
   */
  [[nodiscard]] std::string get_items_filepath()const{return items_filepath;};
  /**
   * @return the output prefix
   */
  [[nodiscard]] std::string get_output_prefix()const{return output_prefix;};
  /**
   * @brief sets the champions filepath
   * @param path the new path to use
   */
  void set_champs_filepath(std::string path){champs_filepath=std::move(path);}
  /**
   * @brief sets the champions filepath
   * @param path the new path to use
   */
  void set_items_filepath(std::string path){items_filepath=std::move(path);}
  /**
   * @brief sets the output prefix
   * @param path the new prefix to use
   */
  void set_output_prefix(std::string prefix){output_prefix=std::move(prefix);}
  /**
   * @brief sets the new gamemode
   * @param mode the new gamemode
   */
  void set_gamemode(GameMode mode_){mode=mode_;}
private:
  std::string champs_filepath;
  std::string items_filepath;
  std::string output_prefix;
  GameMode mode;
};
#endif
