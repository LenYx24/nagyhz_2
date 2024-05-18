#include "../include/game.hpp"
#include <exception>
GameButton::GameButton(Resources::Holder &h, sf::String str, std::function<void()> onclick_, sf::Vector2f pos) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(14);
  text.setFont(h.get(Resources::Type::FONT));
  this->set_position(pos);
}
GameState::GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, const Settings settings, sf::RenderWindow& window) : State(state_manager),map(std::make_unique<Map>(sf::Vector2f{500,20})) {
  // load items from the file and save them to the allitems variable
  iofile inp(settings.items_filepath);
  for (std::string line; std::getline(inp.getfile(), line);) {
    Item item;
    item.readfromstring(line);
    allitems.push_back(item);
  }
  // set gamemode
  this->mode = mode;
  // load font
  h.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");
  // create the UI components:
  sf::Vector2f windowsize = state_manager.get_size(window);
  // todo: get the gamebuttons sizes, and position it accordingly
  buttons.push_back(new GameButton(h, "End my turn", [state = this]() { state->end_turn(); },{80,40}));
  buttons.push_back(new GameButton(h, "back", [&state_manager]() { state_manager.pop_state(); },{80,windowsize.y-50}));

  gamemove_buttons.push_back(new GameButton(h, "move", [state = this]() { state->onclick_gamemove(); }));
  gamemove_buttons.push_back(new GameButton(h, "attack", [state = this]() { state->onclick_attack(); }));
  gamemove_buttons.push_back(new GameButton(h, "ward", [state = this]() { state->onclick_ward(); }));
  gamemove_buttons.push_back(new GameButton(h, "base", [state = this]() { state->onclick_base(); }));

  UI::Grid grid{{windowsize.x/4, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> els(gamemove_buttons.begin(), gamemove_buttons.end());
  grid.setelements(els);
  grid.setelementspos();

  // create the items panel, and list all the items
  sf::RectangleShape itemshape{{100,40}};
  itemshape.setFillColor(sf::Color::Black);
  UI::NamedBox *itemslabel = new UI::NamedBox{"Items:",itemshape,h};
  itemslabel->set_position({windowsize.x - itemslabel->get_global_bounds().width,20});
  itemslabel->setcharsize(12);
  labels.push_back(itemslabel);

  sf::RectangleShape baseshape{{150, 60}};
  baseshape.setOutlineColor({200, 15, 45});
  for (size_t i = 0; i < allitems.size(); i++) {
    std::string itemstats = allitems[i].getname() + "\ndmg: " + std::to_string(allitems[i].getbonusdmg()) + "\nhp: " + std::to_string(allitems[i].getbonushp()) + "\ngold: " + std::to_string(allitems[i].get_gold_value());
    items_boxes.push_back(new ItemBox{itemstats, baseshape, h, &allitems[i]});
    items_boxes[i]->setcharsize(11);
    items_boxes[i]->setlabelcolor(sf::Color::Black);
  }
  UI::Grid itemsgrid{{windowsize.x - baseshape.getSize().x, 60}, {5, 5}, {0, 1}};
  std::vector<UI::GridElement *> itemels(items_boxes.begin(), items_boxes.end());
  itemsgrid.setelements(itemels);
  itemsgrid.setelementspos();

  // set timer
  elapsed_time.restart();
  timer.setPosition({250, 10});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(18);
  // create players
  Player *player_1 = new Player{std::move(p1champs)};
  Player *player_2 = new Player{std::move(p2champs)};
  player_1->setchampicons("ABCDE");
  player_2->setchampicons("FGHIJ");
  player_1->setfont(h);
  player_2->setfont(h);
  // spawn champs
  player_1->setspawnpoint({0,static_cast<float>(map->getcellgridsize().y)-1});
  player_2->setspawnpoint({static_cast<float>(map->getcellgridsize().x)-1,0});
  player_1->spawn_champs(map);
  player_2->spawn_champs(map);
  player_1->set_side(Side::BLUE);
  player_2->set_side(Side::RED);
  create_simulation = [state = this, &window](){
    state->state_manager.push_state(std::make_unique<SimulationState>(state->players, state->map, window, state->mode,state->state_manager));
  };
  
  // seed the random generator
  srand(static_cast<unsigned>(time(0)));
  // Todo: randomly select starter champ
  currentplayer = player_1;
  player_1->set_starter(true);
  selectedchamp = nullptr;

  players.push_back(player_1);
  players.push_back(player_2);
  // update the vision
  map->update_vision();
}
void GameState::next_player(){
  if(players.size() < 2)throw "Not enough players to play the game";
  if(currentplayer == players[0])currentplayer = players[1];
  else currentplayer = players[0];
  map->update_vision_side(currentplayer->get_side());
}
// onclicks:
void GameState::end_turn() {
  // ends the turn and starts the other players turn, or does the simulation
  // a simulation substate needs the map, and the entities on the map
  if(currentplayer->did_start()){
    // the other player is next
    currentplayer->set_simulation(false);
    currentplayer->update_champ_positions(map);
    next_player();
    currentplayer->set_simulation(true);
  }else{
    currentplayer->set_simulation(false);
    currentplayer->update_champ_positions(map);
    // round ends, simulation should start
    create_simulation();
    was_round_end = true;
  }
  // update the vision
  map->update_vision_side(currentplayer->get_side());
  map->update_vision();
}
void GameState::onclick_gamemove() {
  if(GameMove::basic_check(currentplayer,selectedchamp)){
    map->select_accessible_cells(selectedchamp);
    selectedchamp->add_gamemove(new MoveCell);
  }
}
void GameState::onclick_item(Item *selected_item) {
  if(selectedchamp == nullptr)return;
  Cell *spawnpoint = map->getcell(currentplayer->get_spawn_point());
  if(currentplayer->ishischamp(selectedchamp) && selectedchamp->get_simulation_cell() == spawnpoint){
    selectedchamp->add_item(selected_item);
  }
}

void GameState::onclick_attack(){
  if(GameMove::basic_check(currentplayer,selectedchamp)){
    map->select_attackable_entities(selectedchamp);
    selectedchamp->add_gamemove(new AttackMove);
  }
}
void GameState::onclick_base(){
  if(GameMove::basic_check(currentplayer,selectedchamp)){
    selectedchamp->add_gamemove(new TeleportBase);
    selectedchamp->finish_gamemove(map->getcell(currentplayer->get_spawn_point()));
  }
}
void GameState::onclick_ward(){
  if(GameMove::basic_check(currentplayer,selectedchamp)){
    map->select_accessible_cells(selectedchamp);
    selectedchamp->add_gamemove(new PlaceWard);
  }
}
void GameState::show_cellinfo(sf::Vector2f index){
  sf::RectangleShape shape{{100,60}};
  shape.setFillColor(sf::Color::Black);
  stat_labels.clear();
  UI::NamedBox *statlabel = new UI::NamedBox{
      "Cell:\nx: "+std::to_string((int)index.x+1) +
          "\ny: "+std::to_string((int)index.y+1),shape,h};
  statlabel->setcharsize(12);
  statlabel->set_position({70,200});
  stat_labels.push_back(statlabel);
}
void GameState::show_stats(std::vector<std::string> &statsentity){
  sf::Vector2f startpos{70,300};
  sf::RectangleShape shape{{100,20}};
  shape.setFillColor(sf::Color::Black);
  for(size_t i = 0; i < statsentity.size(); i++){
    UI::NamedBox *statlabel = new UI::NamedBox{statsentity[i],shape,h};
    statlabel->set_position(startpos);
    float marginy = 5;
    startpos.y += shape.getSize().y + marginy;
    statlabel->setcharsize(12);
    stat_labels.push_back(statlabel);
  }
}
bool GameState::is_gamemove_finisher(Cell *clickedcell){
  return selectedchamp
         && currentplayer->ishischamp(selectedchamp)
         && !selectedchamp->is_gamemove_complete()
         && clickedcell->is_selected();
}
void GameState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (GameButton *b : buttons) {
      if (b->contains(e.mouseButton.x,e.mouseButton.y)) {
        b->onclick();
        return;
      }
    }
    for (GameButton *b : gamemove_buttons) {
      if (b->contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick();
        return;
      }
    }
    for (ItemBox *b : items_boxes) {
      if (b->contains(e.mouseButton.x, e.mouseButton.y)) {
        // Todo: make itemboxes item a private member
        onclick_item(b->item);
        return;
      }
    }

    Cell *clickedcell = map->getclickedcell(e.mouseButton.x,e.mouseButton.y);
    if(clickedcell != nullptr){
      std::vector<std::string> statsentity;
      
      show_cellinfo(clickedcell->getindex());
      if(is_gamemove_finisher(clickedcell)){
        selectedchamp->finish_gamemove(clickedcell);
        map->move(selectedchamp, selectedchamp->current_gamemove_index(), selectedchamp->last_gamemove_index());
        statsentity = selectedchamp->getstats();
        map->reset_cell_selections();
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
  s += std::to_string(time_left - (int)elapsed_time.getElapsedTime().asSeconds());
  timer.setString(s);
  if (static_cast<int>(elapsed_time.getElapsedTime().asSeconds()) == time_left) {
    elapsed_time.restart();
    end_turn();
  }
  // check if round should end, if the current players champs can't move anymore
  if(currentplayer->check_round_end()){
    end_turn();
  }
  // if in the last main cycle we encountered a round end
  if(was_round_end){
    was_round_end = false;
    next_player();
    for(auto & player : players){
      player->round_end(map);
    }
    elapsed_time.restart();
  }
  // check game end
  if(map->did_game_end()){
    state_manager.pop_state();
  }
}
void GameState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (auto & button : buttons) {
    button->draw_to_window(window);
  }
  for (auto & gamemovebutton : gamemove_buttons) {
    gamemovebutton->draw_to_window(window);
  }
  for(auto & label : labels){
    label->draw(window);
  }
  for(auto & statlabel : stat_labels){
    statlabel->draw(window);
  }
  for(auto & i : items_boxes){
    i->draw(window);
  }
  map->draw(window);
  window.draw(timer);
}
GameState::~GameState(){
  for(auto & player : players){
    delete player;
  }
  for(auto & button : buttons){
    delete button;
  }
  for(auto & gamemovebutton : gamemove_buttons){
    delete gamemovebutton;
  }
  for(auto & label : stat_labels){
    delete label;
  }
  for(auto & label : labels){
    delete label;
  }
  for(auto & item : items_boxes){
    delete item;
  }
}