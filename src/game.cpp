#include "../include/game.hpp"
GameButton::GameButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_, sf::Vector2f pos) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(14);
  text.setFont(h.get(Resources::Type::FONT));
  this->set_position(pos);
}
GameState::GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, GameMode mode, sf::RenderWindow& window) : State(state_manager),map(std::make_unique<Map>(sf::Vector2f{500,20})) {
  // load items from the file and save them to the allitems variable
  iofile inp("examples/items.txt");
  for (std::string line; std::getline(inp.getfile(), line);) {
    Item item;
    item.readfromstring(line);
    allitems.push_back(item);
  }
  // set gamemode
  this->mode = mode;
  // load font
  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  // create the UI components:
  sf::Vector2f windowsize = state_manager.get_size(window);
  // todo: get the gamebuttons sizes, and position it accordingly
  buttons.push_back(new GameButton(h, "End my turn", [state = this](StateManager &s) { state->end_turn(); },{80,40}));
  buttons.push_back(new GameButton(h, "back", [](StateManager &s) { s.pop_state(); },{80,windowsize.y-50}));

  gamemovebuttons.push_back(new GameButton(h, "move", [state = this](StateManager &s) { state->onclick_gamemove(); }));
  gamemovebuttons.push_back(new GameButton(h, "attack", [state = this](StateManager &s) { state->onclick_attack(); }));
  gamemovebuttons.push_back(new GameButton(h, "ward", [state = this](StateManager &s) { state->onclick_ward(); }));
  gamemovebuttons.push_back(new GameButton(h, "base", [state = this](StateManager &s) { state->onclick_base(); }));
  UI::Grid grid{{windowsize.x/4, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> els(gamemovebuttons.begin(), gamemovebuttons.end());
  grid.setelements(els);
  grid.setelementspos();

  // create the items panel, and list all the items
  sf::RectangleShape itemshape{{100,40}};
  itemshape.setFillColor(sf::Color::Black);
  UI::NamedBox *itemslabel = new UI::NamedBox{"Items:",itemshape,h};
  itemslabel->set_position({windowsize.x - itemslabel->getglobalbounds().width,20});
  itemslabel->setcharsize(12);
  labels.push_back(itemslabel);

  sf::RectangleShape baseshape{{150, 60}};
  baseshape.setOutlineColor({200, 15, 45});
  for (size_t i = 0; i < allitems.size(); i++) {
    std::string itemstats = allitems[i].getname() + "\ndmg: " + std::to_string(allitems[i].getbonusdmg()) + "\nhp: " + std::to_string(allitems[i].getbonushp()) + "\ngold: " + std::to_string(allitems[i].get_gold_value());
    itemslist.push_back(new ItemBox{itemstats, baseshape, h, &allitems[i]});
    itemslist[i]->setcharsize(11);
    itemslist[i]->setlabelcolor(sf::Color::Black);
  }
  UI::Grid itemsgrid{{windowsize.x - baseshape.getSize().x, 60}, {5, 5}, {0, 1}};
  std::vector<UI::GridElement *> itemels(itemslist.begin(), itemslist.end());
  itemsgrid.setelements(itemels);
  itemsgrid.setelementspos();

  // set timer
  elapsedtime.restart();
  timer.setPosition({250, 10});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(18);
  // create players
  players.push_back(new Player{p1champs});
  players.push_back(new Player{p2champs});
  players[0]->setchampicons("ABCDE");
  players[1]->setchampicons("FGHIJ");
  players[0]->setfont(h);
  players[1]->setfont(h);
  // spawn champs
  players[0]->setspawnpoint({0,map->getcellgridsize().y-1});
  players[1]->setspawnpoint({map->getcellgridsize().x-1,0});
  players[0]->spawnchamps(map);
  players[1]->spawnchamps(map);
  players[0]->set_side(Side::BLUE);
  players[1]->set_side(Side::RED);
  // spawn minions
  create_simulation = [state = this, &window](){
    state->state_manager.push_state(std::make_unique<SimulationState>(state->players, state->map, window, state->mode,state->state_manager));
  };
  
  // seed the random generator
  srand(static_cast<unsigned>(time(0)));
  // Todo: randomly select starter champ
  currentplayer = players[0];
  players[0]->set_starter(true);
  selectedchamp = nullptr;

  std::cout << "[[info]] initalized gamestate" << std::endl;
}
// onclicks:
void GameState::end_turn() {
  // ends the turn and starts the other players turn, or does the simulation
  // a simulation substate needs the map, and the entities on the map
  // it also needs to know, who's able to see the moves
  if(currentplayer->did_start()){
    // the other player is next
    currentplayer->set_simulation(false);
    currentplayer->update_champ_positions(map);
    if(currentplayer == players[0])currentplayer = players[1];
    else currentplayer = players[0];
    currentplayer->set_simulation(true);
  }else{
    currentplayer->set_simulation(false);
    currentplayer->update_champ_positions(map);
    // round ends, simulation should start
    std::cout << "round ends" << std::endl;
    create_simulation();
    for(size_t i = 0; i < players.size(); i++){
      players[i]->round_end(map);
    }
  }
  elapsedtime.restart();
}
void GameState::onclick_gamemove() {
  MoveCell *move_cell = new MoveCell;
  move_cell->onclick(map,currentplayer,selectedchamp);
}
void GameState::onclick_item(Item *selected_item) {
  if(selectedchamp == nullptr)return;
  Cell *spawnpoint = map->getcell(currentplayer->get_spawn_point());
  if(currentplayer->ishischamp(selectedchamp) && selectedchamp->get_simulation_cell() == spawnpoint){
    selectedchamp->add_item(selected_item);
  }
}

void GameState::onclick_attack(){

}
void GameState::onclick_base(){
  TeleportBase *teleport = new TeleportBase;
  teleport->onclick(map,currentplayer,selectedchamp);
}
void GameState::onclick_ward(){
  PlaceWard *ward = new PlaceWard;
  ward->onclick(map,currentplayer,selectedchamp);
}
void GameState::show_cellinfo(sf::Vector2f index){
  sf::RectangleShape shape{{100,60}};
  shape.setFillColor(sf::Color::Black);
  statlabels.clear();
  UI::NamedBox *statlabel = new UI::NamedBox{"Cell:\nx: "+std::to_string((int)index.x+1) + "\ny: "+std::to_string((int)index.y+1),shape,h}; 
  statlabel->setcharsize(12);
  statlabel->set_position({70,200});
  statlabels.push_back(statlabel);
}
void GameState::show_stats(std::vector<std::string> &statsentity){
  sf::Vector2f startpos{70,300};
  sf::RectangleShape shape{{100,60}};
  shape.setFillColor(sf::Color::Black);
  for(size_t i = 0; i < statsentity.size(); i++){
    UI::NamedBox *statlabel = new UI::NamedBox{statsentity[i],shape,h};
    statlabel->set_position(startpos);
    int marginy = 20;
    startpos.y += shape.getSize().y + marginy;
    statlabel->setcharsize(12);
    statlabels.push_back(statlabel);
  }
}
bool GameState::is_gamemove_finisher(Cell *clickedcell){
  return selectedchamp && currentplayer->ishischamp(selectedchamp) && !selectedchamp->is_gamemove_complete() && clickedcell->is_selected();
}
//void Round::roundend(){}
void GameState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (GameButton *b : buttons) {
      if (b->get_global_bounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(state_manager);
        return;
      }
    }
    for (GameButton *b : gamemovebuttons) {
      if (b->get_global_bounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(state_manager);
        return;
      }
    }
    for (ItemBox *b : itemslist) {
      if (b->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        // Todo: make itemboxes item a private member
        onclick_item(b->item);
        return;
      }
    }

    Cell *clickedcell = map->getclickedcell(e.mouseButton.x,e.mouseButton.y);
    if(clickedcell != nullptr){
      map->reset_cell_selections();
      std::vector<std::string> statsentity;
      
      show_cellinfo(clickedcell->getindex());
      if(is_gamemove_finisher(clickedcell)){
        selectedchamp->finish_gamemove(clickedcell);
        map->move(selectedchamp, selectedchamp->current_gamemove_index(), selectedchamp->last_gamemove_index());
        statsentity = selectedchamp->getstats();
      }
      else{
        clickedcell->sethighlighted();
        Entity *clickedentity = clickedcell->getentitiyclicked(e.mouseButton.x, e.mouseButton.y);
        if(clickedentity != nullptr){
          selectedchamp = currentplayer->getselectedchamp(clickedcell->getindex());
          statsentity = clickedentity->getstats();
        }
      }
      show_stats(statsentity);
    }
  }
}
void GameState::update() {
  // update time elapsed
  std::string s = "Time left: ";
  int timeleft = 60;
  s += std::to_string(timeleft - (int)elapsedtime.getElapsedTime().asSeconds());
  timer.setString(s);
  if (static_cast<int>(elapsedtime.getElapsedTime().asSeconds()) == timeleft) {
    elapsedtime.restart();
    // end turn
  }
  // check round end
  if(currentplayer->check_round_end()){
    end_turn();
  }
}
void GameState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->draw_to_window(window);
  }
  for (size_t i = 0; i < gamemovebuttons.size(); i++) {
    gamemovebuttons[i]->draw_to_window(window);
  }
  for(size_t i = 0; i < labels.size(); i++){
    labels[i]->draw(window);
  }
  for(size_t i = 0; i < statlabels.size(); i++){
    statlabels[i]->draw(window);
  }
  for(size_t i = 0; i < itemslist.size(); i++){
    itemslist[i]->draw(window);
  }
  map->draw(window);
  window.draw(timer);
}
GameState::~GameState(){
  for(size_t i = 0; i < players.size(); i++){
    delete players[i];
  }
  // this is buggy yet
  // for(size_t i = 0; i < labels.size(); i++){
  //   delete labels[i];
  // }
  for(size_t i = 0; i < buttons.size(); i++){
    delete buttons[i];
  }
  for(size_t i = 0; i < gamemovebuttons.size(); i++){
    delete gamemovebuttons[i];
  }
}