#include "../include/game.hpp"
GameButton::GameButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}
GameState::GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, GameMode mode) : State(state_manager),map(std::make_unique<Map>(sf::Vector2f{150,10})) {
  // load items from the file and save them to the allitems variable
  std::cout << "hello"<<std::endl;
  iofile inp("examples/items.txt");
  for (std::string line; std::getline(inp.getfile(), line);) {
    Item item;
    item.readfromstring(line);
    allitems.push_back(item);
  }
  // set gamemode
  _mode = mode;
  // load font
  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  // create players
  players.push_back(new Player{p1champs});
  players.push_back(new Player{p2champs});
  std::cout << "init done" << std::endl;
  // create the UI components:
  buttons.push_back(new GameButton(h, "Lock in", [state = this](StateManager &s) { state->rounds[0].roundend(); }));
  buttons.push_back(new GameButton(h, "back", [](StateManager &s) { s.PopState(); }));
  // create selected champ panel, with stats and items
  // list the moves as buttons, and the end round button
  // show a label, that shows the current points for the player
  // show a textbox, where the player can give his input
  // create a map, with create_map method, with the basic entities on it
  // create the items panel, and list all the items

  // set timer
  elapsedtime.restart();
  timer.setPosition({40, 40});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(10);
  // start a round
  // seed the random generator
}
// onclicks:
void onclick_endturn() {
  // ends the turn and starts the other players turn, or does the simulation
  // a simulation substate needs the map, and the entities on the map
  // it also needs to know, who's able to see the moves
}
void onclick_gamemove() {
  // checks if a champion is selected
  // gets the current cell, where the champion is right now
  // asks the gamemove, to make certain cells clickable, which are valid
}
void onclick_cell() {
  // checks if there's a selected move right now
  // if there's, then if the cell was clickable, then add the gamemove to the currently selected champions gamemove
  // reset the timer
  // reset clickable cells
}
void onclick_champ() {
  // update selected champs stats
  // update the available game moves
}
void onclick_item() {
  // check if there's currently a selected champion
  // tries to add the item to the champion (the checks are made by the champion class)
}
void Round::roundend(){}
void GameState::HandleEvents(sf::Event &e) {
  // check if user clicked on an element, then do the task accordingly
  if (e.type == sf::Event::Closed) {
    _state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (GameButton *b : buttons) {
      if (b->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(_state_manager);
      }
    }
  }
}
void GameState::Update() {
  // update time elapsed
  std::string s = "Ido: ";
  s += std::to_string(30 - (int)elapsedtime.getElapsedTime().asSeconds());
  timer.setString(s);
  if ((int)elapsedtime.getElapsedTime().asSeconds() == 30) {
    elapsedtime.restart();
    // end turn
  }
}
void GameState::Draw(sf::RenderWindow &window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->draw_to_window(window);
  }
  this->map->draw(window);
  window.draw(timer);
}
GameState::~GameState(){
  for(size_t i = 0; i < players.size(); i++){
    delete players[i];
  }
}