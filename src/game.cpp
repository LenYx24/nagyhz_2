#include "../include/game.hpp"

ItemBox::ItemBox(const std::string& label, [[maybe_unused]]sf::RectangleShape frame, Resources::Holder &h, Item *i)
    : NamedBox(label, std::move(frame), h), item(i) {}

GameButton::GameButton(Resources::Holder &h,
                       const sf::String& str,
                       [[maybe_unused]]std::function<void()> onclick_,
                       sf::Vector2f pos) : Button(str, std::move(onclick_)) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(14);
  text.setFont(h.get(Resources::Type::FONT));
  text.setString(str);
  this->set_position(pos);
}
GameState::GameState(StateManager &state_manager,
                     std::vector<Champion*> p1champs,
                     std::vector<Champion*> p2champs,
                     Settings& settings,
                     sf::RenderWindow& window) :
      State(state_manager),
      settings(settings),
      map(std::make_unique<Map>(sf::Vector2f{500,20}))
      {
  // load items from the file and save them to the all items variable
  IOParser::File input(settings.get_items_filepath());
  for (std::string line; std::getline(input.getfile(), line);) {
    all_items.push_back(IOParser::create_item(line));
  }
  std::time_t time = std::time(nullptr);
  std::tm *now = std::localtime(&time);
  std::string filename = this->settings.get_output_prefix();
  filename+="_";

  filename+= std::to_string(now->tm_year)
              + ":"+std::to_string(now->tm_mon+1)
              + ":"+std::to_string(now->tm_mday)
              + ":"+std::to_string(now->tm_hour)
              + ":"+std::to_string(now->tm_min)
              + ":"+std::to_string(now->tm_sec);
  filename += ".txt";
  output_file = std::ofstream(filename);
  if(!output_file){
    throw std::invalid_argument("wrong filepath");
  }
  // load font
  h.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");
  // create the UI components:
  sf::Vector2f window_size = StateManager::get_size(window);

  buttons.push_back(new GameButton(h, "End my turn",
                                   [state = this]() {state->end_turn();},
                                   {80,40}));
  buttons.push_back(new GameButton(h, "back",
                                   [&state_manager]() { state_manager.pop_state(); },
                                   {80, window_size.y-50}));

  gamemove_buttons.push_back(new GameButton(h, "move 1 points",
                                            [state = this]() { state->onclick_movecell(); }));
  gamemove_buttons.push_back(new GameButton(h, "attack 2 points",
                                            [state = this]() { state->onclick_attack(); }));
  gamemove_buttons.push_back(new GameButton(h, "ward 2 points",
                                            [state = this]() { state->onclick_ward(); }));
  gamemove_buttons.push_back(new GameButton(h, "base 3 points",
                                            [state = this]() { state->onclick_base(); }));
  gamemove_buttons.push_back(new GameButton(h, "reset gamemove",
                                            [state = this]() { state->onclick_reset_gamemove(); }));

  UI::Grid grid{{window_size.x/4, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> elements(gamemove_buttons.begin(), gamemove_buttons.end());
  grid.set_elements(elements);
  grid.set_elements_pos();

  // create the items panel, and list all the items
  sf::RectangleShape items_title_shape{{100,40}};
  items_title_shape.setFillColor(sf::Color::Black);
  auto *items_label = new UI::NamedBox{"Items:", items_title_shape,h};
  items_label->set_position({window_size.x - items_label->get_global_bounds().width,20});
  items_label->set_char_size(12);
  labels.push_back(items_label);

  sf::RectangleShape item_shape{{150, 60}};
  item_shape.setOutlineColor({200, 15, 45});
  for (size_t i = 0; i < all_items.size(); i++) {
    std::string item_stats = all_items[i].get_name() +
                             "\ndmg: " + std::to_string(all_items[i].get_bonus_dmg()) +
                             "\nhp: " + std::to_string(all_items[i].get_bonus_hp()) +
                             "\ngold: " + std::to_string(all_items[i].get_gold_value());
    items_boxes.push_back(new ItemBox{item_stats, item_shape, h, &all_items[i]});
    items_boxes[i]->set_char_size(11);
    items_boxes[i]->set_label_color(sf::Color::Black);
  }
  UI::Grid items_grid{{
                          window_size.x - item_shape.getSize().x, 60},
                      {5, 5},
                      {0, 1}
  };
  std::vector<UI::GridElement *> item_elements(items_boxes.begin(), items_boxes.end());
  items_grid.set_elements(item_elements);
  items_grid.set_elements_pos();

  // set timer
  elapsed_time.restart();
  timer.setPosition({250, 10});
  timer.setFont(h.get(Resources::Type::FONT));
  timer.setCharacterSize(18);
  timer.setFillColor({sf::Color::Black});
  // create players
  auto *player_1 = new Player{std::move(p1champs)};
  auto *player_2 = new Player{std::move(p2champs)};
  player_1->set_champ_icons("ABCDE");
  player_2->set_champ_icons("FGHIJ");
  player_1->set_font(h);
  player_2->set_font(h);
  // spawn champs
  Cell *player1_spawn_point = map->getcell({0, static_cast<float>(map->get_cell_grid_size().y) - 1});
  Cell *player2_spawn_point = map->getcell({static_cast<float>(map->get_cell_grid_size().x) - 1, 0});
  player_1->set_spawn_point(player1_spawn_point);
  player_2->set_spawn_point(player2_spawn_point);
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
    state->selected_champ = nullptr;
  };
  create_simulation = [state = this, &window, simulation_ended](){
    state->state_manager.push_state(std::make_unique<SimulationState>(
        state->players, state->map,
        window,
        state->settings,
        state->state_manager,
        state->output_file,
        simulation_ended)
    );
  };
  // set the current player
  current_player = player_1;
  current_player->set_starter(true);
  selected_champ = nullptr;

  players.push_back(player_1);
  players.push_back(player_2);
  // update the vision
  map->update_vision_side(current_player->get_side());
  map->update_vision();
}
void GameState::next_player(){
  if(current_player == nullptr){
    if(players[0]->did_start())
      current_player = players[0];
    else
      current_player = players[1];
  }
  else{
    if(current_player == players[0])
      current_player = players[1];
    else
      current_player = players[0];
  }
  // update the vision when changing players, to show their respective vision
  map->update_vision_side(current_player->get_side());
  map->update_vision();
  // restart time
  elapsed_time.restart();
}
void GameState::end_turn() {
  // ends the turn and starts the other players turn, or does the simulation
  // a simulation substate needs the map, and the entities on the map
  if(current_player->did_start()){
    // the other player is next
    current_player->set_simulation(false);
    current_player->update_champ_positions(map);
    next_player();
    current_player->set_simulation(true);
  }else{
    current_player->set_simulation(false);
    current_player->update_champ_positions(map);
    // round ends, simulation should start
    create_simulation();
  }
}
void GameState::onclick_item(Item *selected_item) {
  if(selected_champ == nullptr)return;
  if(current_player->is_his_champ(selected_champ)
      && selected_champ->get_simulation_cell() == current_player->get_spawn_point()){
    selected_champ->add_item(selected_item);
    auto stats = selected_champ->get_stats();
    show_stats(stats);
  }
}
void GameState::onclick_reset_gamemove(){
  if(selected_champ && current_player->is_his_champ(selected_champ)){
    selected_champ->remove_last_gamemove();
    map->reset_cell_selections();
  }
}

void GameState::onclick_movecell() {
  auto move = new MoveCell;
  if(move->check_gamemove_addable(current_player, selected_champ)){
    map->select_accessible_cells(selected_champ);
    selected_champ->add_gamemove(move);
  }
  else delete move;
}

void GameState::onclick_attack(){
  auto move = new AttackMove;
  if(move->check_gamemove_addable(current_player, selected_champ)){
    map->select_attackable_entities(selected_champ);
    selected_champ->add_gamemove(move);
  }
  else delete move;
}
void GameState::after_gamemove(){
  selected_champ->move(map);
  auto stats = selected_champ->get_stats();
  map->update_vision_side(current_player->get_side());
  map->update_vision();
  map->reset_cell_selections();
  show_stats(stats);
}
void GameState::onclick_base(){
  auto move = new TeleportBase;
  if(move->check_gamemove_addable(current_player, selected_champ)){
    selected_champ->add_gamemove(move);
    selected_champ->finish_gamemove(current_player->get_spawn_point());
    after_gamemove();
  }
  else delete move;
}
void GameState::onclick_ward(){
  auto move = new PlaceWard;
  if(move->check_gamemove_addable(current_player, selected_champ)){
    map->select_wardable_cells(selected_champ);
    selected_champ->add_gamemove(move);
  }
  else delete move;
}
void GameState::show_cell_info(sf::Vector2f index){
  sf::RectangleShape shape{{140,50}};
  shape.setFillColor(sf::Color::Black);
  for(auto & label : stat_labels){
    delete label;
  }
  stat_labels.clear();
  int row = static_cast<int>(index.x)+1;
  int col = static_cast<int>(index.y)+1;
  auto *stat_label = new UI::NamedBox{
      "Cell:"
          "\nx: "+std::to_string(row)+
          "\ny: "+std::to_string(col),shape,h};
  stat_label->set_char_size(12);
  stat_label->set_position({70,100});
  stat_labels.push_back(stat_label);
}
void GameState::show_stats(std::vector<std::string> &stats){
  sf::Vector2f startpos{70,200};
  sf::RectangleShape shape{{100,30}};
  shape.setFillColor(sf::Color::Black);
  for(const auto & i : stats){
    auto *stat_label = new UI::NamedBox{i,shape,h};
    stat_label->set_position(startpos);
    float margin_y = 5;
    startpos.y += shape.getSize().y + margin_y;
    stat_label->set_char_size(12);
    stat_labels.push_back(stat_label);
  }
}
bool GameState::is_gamemove_finisher(Cell *clicked_cell){
  return selected_champ && current_player->is_his_champ(selected_champ)
         && !selected_champ->is_gamemove_complete()
         && clicked_cell->is_selected();
}
void GameState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (GameButton *b : buttons) {
      try{
        b->onclick_here(e);
      }catch(const std::runtime_error& err){
        std::cout << err.what() << std::endl;
        state_manager.exit();
        return;
      }
    }
    for (GameButton *b : gamemove_buttons) {
      b->onclick_here(e);
    }
    for (ItemBox *b : items_boxes) {
      if (b->contains(e.mouseButton.x, e.mouseButton.y)) {
        onclick_item(b->get_item());
        return;
      }
    }

    Cell *clicked_cell = map->get_clicked_cell(e.mouseButton.x, e.mouseButton.y);
    if(clicked_cell != nullptr){
      std::vector<std::string> stats;

      show_cell_info(clicked_cell->get_index());
      if(is_gamemove_finisher(clicked_cell)){
        selected_champ->finish_gamemove(clicked_cell);
        after_gamemove();
      }
      // only reset cell selections, if a gamemove is complete
      else if((selected_champ && selected_champ->is_gamemove_complete()) || !selected_champ){
        map->reset_cell_selections();
        map->update_vision();

        clicked_cell->set_highlighted();
        Entity *clicked_entity = clicked_cell->get_entity_clicked(e.mouseButton.x, e.mouseButton.y);
        if(clicked_entity != nullptr){
          selected_champ = current_player->get_selected_champs(clicked_cell->get_index());
          stats = clicked_entity->get_stats();
        }
      }
      show_stats(stats);
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
      || current_player->check_round_end()) {
    try{
      end_turn();
    }catch(const std::runtime_error& err){
      std::cout << err.what() << std::endl;
      state_manager.exit();
      return;
    }
  }
  // check game end
  if(map->check_game_end()){
    state_manager.pop_state();
  }
}
void GameState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (auto & button : buttons) {
    button->draw(window);
  }
  for (auto & gamemovebutton : gamemove_buttons) {
    gamemovebutton->draw(window);
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
    player->despawn_from_map(map);
    delete player;
  }
}