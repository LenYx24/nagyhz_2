#include "../include/game.hpp"
#include <exception>
GameButton::GameButton(Resources::Holder &h, const sf::String& str, std::function<void()> onclick_, sf::Vector2f pos) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(14);
  text.setFont(h.get(Resources::Type::FONT));
  text.setString(str);
  this->set_position(pos);
}
GameState::GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, Settings settings, sf::RenderWindow& window) : State(state_manager),map(std::make_unique<Map>(sf::Vector2f{500,20})) {
  // load items from the file and save them to the allitems variable
  iofile inp(settings.items_filepath);
  for (std::string line; std::getline(inp.getfile(), line);) {
    Item item;
    item.read_from_string(line);
    allitems.push_back(item);
  }
  // set gamemode
  this->mode = settings.mode;
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
  gamemove_buttons.push_back(new GameButton(h, "reset gamemove", [state = this]() { state->onclick_reset_gamemove(); }));

  UI::Grid grid{{windowsize.x/4, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> els(gamemove_buttons.begin(), gamemove_buttons.end());
  grid.set_elements(els);
  grid.set_elements_pos();

  // create the items panel, and list all the items
  sf::RectangleShape itemshape{{100,40}};
  itemshape.setFillColor(sf::Color::Black);
  UI::NamedBox *itemslabel = new UI::NamedBox{"Items:",itemshape,h};
  itemslabel->set_position({windowsize.x - itemslabel->get_global_bounds().width,20});
  itemslabel->set_char_size(12);
  labels.push_back(itemslabel);

  sf::RectangleShape baseshape{{150, 60}};
  baseshape.setOutlineColor({200, 15, 45});
  for (size_t i = 0; i < allitems.size(); i++) {
    std::string itemstats = allitems[i].get_name() + "\ndmg: " + std::to_string(allitems[i].get_bonus_dmg()) + "\nhp: " + std::to_string(allitems[i].get_bonus_hp()) + "\ngold: " + std::to_string(allitems[i].get_gold_value());
    items_boxes.push_back(new ItemBox{itemstats, baseshape, h, &allitems[i]});
    items_boxes[i]->set_char_size(11);
    items_boxes[i]->set_label_color(sf::Color::Black);
  }
  UI::Grid itemsgrid{{windowsize.x - baseshape.getSize().x, 60}, {5, 5}, {0, 1}};
  std::vector<UI::GridElement *> itemels(items_boxes.begin(), items_boxes.end());
  itemsgrid.set_elements(itemels);
  itemsgrid.set_elements_pos();

  // set timer
  elapsed_time.restart();
  timer.setPosition({250, 10});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(18);
  // create players
  Player *player_1 = new Player{std::move(p1champs)};
  Player *player_2 = new Player{std::move(p2champs)};
  player_1->set_champ_icons("ABCDE");
  player_2->set_champ_icons("FGHIJ");
  player_1->set_font(h);
  player_2->set_font(h);
  // spawn champs
  player_1->set_spawn_point({0, static_cast<float>(map->get_cell_grid_size().y) - 1});
  player_2->set_spawn_point({static_cast<float>(map->get_cell_grid_size().x) - 1, 0});
  player_1->spawn_champs(map);
  player_2->spawn_champs(map);
  player_1->set_side(Side::BLUE);
  player_2->set_side(Side::RED);

  // the callback functions that will be called after simulation state ends
  std::function<void()> simulation_ended = [state=this](){
    state->next_player();
    for(auto & player : state->players){
      player->round_end(state->map);
    }
    state->elapsed_time.restart();
    state->selectedchamp = nullptr;
  };
  create_simulation = [state = this, &window, simulation_ended](){
    state->state_manager.push_state(std::make_unique<SimulationState>(
        state->players, state->map, window, state->mode,state->state_manager,simulation_ended)
    );
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
  map->update_vision_side(currentplayer->get_side());
  map->update_vision();
}
void GameState::next_player(){
  if(currentplayer == nullptr){
    if(players[0]->did_start())currentplayer = players[0];
    else currentplayer = players[1];
  }
  else{
    if(currentplayer == players[0])currentplayer = players[1];
    else currentplayer = players[0];
  }
  // update the vision when changing players, to show their respective vision
  map->update_vision_side(currentplayer->get_side());
  map->update_vision();
  // restart time
  elapsed_time.restart();
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
  }
}
void GameState::onclick_item(Item *selected_item) {
  if(selectedchamp == nullptr)return;
  Cell *spawnpoint = map->getcell(currentplayer->get_spawn_point());
  if(currentplayer->is_his_champ(selectedchamp) && selectedchamp->get_simulation_cell() == spawnpoint){
    selectedchamp->add_item(selected_item);
    auto stats = selectedchamp->get_stats();
    show_stats(stats);
  }
}
void GameState::onclick_reset_gamemove(){
  if(selectedchamp && currentplayer->is_his_champ(selectedchamp)){
    selectedchamp->remove_last_gamemove();
    map->reset_cell_selections();
  }
}

void GameState::onclick_gamemove() {
  MoveCell move_cell;
  if(move_cell.check_gamemove_addable(currentplayer, selectedchamp)){
    map->select_accessible_cells(selectedchamp);
    selectedchamp->add_gamemove(new MoveCell);
  }
}

void GameState::onclick_attack(){
  AttackMove attack_move;
  if(attack_move.check_gamemove_addable(currentplayer, selectedchamp)){
    map->select_attackable_entities(selectedchamp);
    selectedchamp->add_gamemove(new AttackMove);
  }
}
void GameState::onclick_base(){
  TeleportBase tp_base;
  if(tp_base.check_gamemove_addable(currentplayer, selectedchamp)){
    selectedchamp->add_gamemove(new TeleportBase);
    selectedchamp->finish_gamemove(map->getcell(currentplayer->get_spawn_point()));
  }
}
void GameState::onclick_ward(){
  PlaceWard place_ward;
  if(place_ward.check_gamemove_addable(currentplayer, selectedchamp)){
    map->select_wardable_cells(selectedchamp);
    selectedchamp->add_gamemove(new PlaceWard);
  }
}
void GameState::show_cellinfo(sf::Vector2f index){
  sf::RectangleShape shape{{100,60}};
  shape.setFillColor(sf::Color::Black);
  for(auto & label : stat_labels){
    delete label;
  }
  stat_labels.clear();
  int row = static_cast<int>(index.x)+1;
  int col = static_cast<int>(index.y)+1;
  UI::NamedBox *statlabel = new UI::NamedBox{
      "Cell:"
          "\nx: "+std::to_string(row)+
          "\ny: "+std::to_string(col),shape,h};
  statlabel->set_char_size(12);
  statlabel->set_position({70,200});
  stat_labels.push_back(statlabel);
}
void GameState::show_stats(std::vector<std::string> &statsentity){
  sf::Vector2f startpos{70,300};
  sf::RectangleShape shape{{100,30}};
  shape.setFillColor(sf::Color::Black);
  for(const auto & i : statsentity){
    UI::NamedBox *statlabel = new UI::NamedBox{i,shape,h};
    statlabel->set_position(startpos);
    float margin_y = 5;
    startpos.y += shape.getSize().y + margin_y;
    statlabel->set_char_size(12);
    stat_labels.push_back(statlabel);
  }
}
bool GameState::is_gamemove_finisher(Cell *clickedcell){
  return selectedchamp
         && currentplayer->is_his_champ(selectedchamp)
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
        onclick_item(b->item);
        return;
      }
    }

    Cell *clickedcell = map->get_clicked_cell(e.mouseButton.x, e.mouseButton.y);
    if(clickedcell != nullptr){
      std::vector<std::string> statsentity;
      
      show_cellinfo(clickedcell->get_index());
      if(is_gamemove_finisher(clickedcell)){
        selectedchamp->finish_gamemove(clickedcell);
        selectedchamp->move(map);
        statsentity = selectedchamp->get_stats();
        map->update_vision_side(currentplayer->get_side());
        map->update_vision();
        map->reset_cell_selections();
      }
      // only reset cell selections, if a gamemove is complete
      else if((selectedchamp && selectedchamp->is_gamemove_complete()) || !selectedchamp){
        map->reset_cell_selections();
        map->update_vision();

        clickedcell->set_highlighted();
        Entity *clickedentity = clickedcell->get_entity_clicked(e.mouseButton.x, e.mouseButton.y);
        if(clickedentity != nullptr){
          selectedchamp = currentplayer->get_selected_champs(clickedcell->get_index());
          statsentity = clickedentity->get_stats();
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
  // check if time elapsed or
  // check if round should end, if the current players champs can't move anymore
  if (static_cast<int>(elapsed_time.getElapsedTime().asSeconds()) == time_left
      || currentplayer->check_round_end()) {
    end_turn();
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
  for(auto & player : players){
    player->despawn_champs(map);
    delete player;
  }
}