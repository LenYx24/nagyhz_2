#include "../include/simulation.hpp"

SimulationState::SimulationState(std::vector<Player *> &players, std::shared_ptr<Map> map, sf::RenderWindow& window, GameMode mode, StateManager& state_manager):State(state_manager),players(players){
  // set gamemode
  this->mode = mode;
  this->map = map;
  // load font
  h.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");
  // create the UI components:
  //sf::Vector2f windowsize = state_manager.get_size(window);

  // create the items panel, and list all the items
  
  // set timer
  elapsed_time.restart();
  timer.setPosition({250, 10});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(18);
  // Todo: find out if setting the font is needed
//   players[0]->setfont(h);
//   players[1]->setfont(h);
    round_counter = 0;
    round_count = 3;
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
        for(size_t i = 0; i < players.size(); i++){
            players[i]->domoves(map);
        }
        if(round_counter == round_count){
            state_manager.pop_state();
        }
        round_counter++;
    }
}
void SimulationState::draw(sf::RenderWindow& window){
    sf::Color background_color = sf::Color(220, 225, 222);
    window.clear(background_color);
    map->draw(window);
    window.draw(timer);
}