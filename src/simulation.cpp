#include <utility>

#include "../include/simulation.hpp"

SimulationState::SimulationState(
    std::vector<Player *> &players,
    std::shared_ptr<Map> &map,
    sf::RenderWindow& window,
    GameMode mode, StateManager& state_manager,
    std::function<void()> callback_):State(state_manager),players(players){
  this->mode = mode;
  this->map = map;
  this->callback = std::move(callback_);
  // load font
  h.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");
  // create the UI components:
  sf::Vector2f window_size = state_manager.get_size(window);
  title = new UI::NamedBox{h};
  title->set_position({window_size.x/2.f - 150,10});
  // set timer
  elapsed_time.restart();
  timer.setPosition({250, 10});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(18);
  // reset vision and selections, as they are not needed in simulation state
  map->reset_cell_selections();
  map->reset_cell_vision();
};
void SimulationState::handle_events(sf::Event &e){
    if (e.type == sf::Event::Closed) {
        state_manager.exit();
    }
}
void SimulationState::update(){
    std::string s = "Time left: ";
    int timeleft = 2;
    s += std::to_string(timeleft - (int)elapsed_time.getElapsedTime().asSeconds());
    timer.setString(s);
    if (static_cast<int>(elapsed_time.getElapsedTime().asSeconds()) == timeleft) {
        elapsed_time.restart();
        // do one turn
        for(auto & player : players){
          player->do_moves(map);
        }
        if(map->did_game_end()){
          state_manager.pop_state();
        }
        if(round_counter == round_count){
            state_manager.pop_state();
        }
        round_counter++;
    }
    // reset vision
    map->reset_cell_selections();
    map->disable_vision();
}
SimulationState::~SimulationState(){
  delete title;
  // no gamemoves should be on any champion, this is just to make sure all of them get deleted
  for(auto player: players){
    player->clear_gamemoves();
  }
  callback();
}
void SimulationState::draw(sf::RenderWindow& window){
    sf::Color background_color = sf::Color(220, 225, 222);
    window.clear(background_color);
    map->draw(window);
    title->draw(window);
    window.draw(timer);
}