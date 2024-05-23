#include "../include/draft.hpp"

void onclick_back(StateManager &s) {
  s.pop_state();
}

void DraftState::lockin(StateManager &s, sf::RenderWindow& window, Settings &settings) {
  if (selected_champ != nullptr) {
    elapsed_time.restart();
    turns[turn_counter++].do_turn(selected_champ);
    auto it = champ_list.begin();
    for(;it != champ_list.end();it++){
      if((*it)->get_champ()->get_name() == selected_champ->get_name()){
        champ_list.erase(it);
        break;
      }
    }
    selected_champ = nullptr;
  }
  if(turn_counter == 20){
    std::vector<Champion *> p1champs{columns[0].champs_size()};
    std::vector<Champion *> p2champs{columns[1].champs_size()};
    for(size_t i = 0; i < columns[0].champs_size(); i++){
      p1champs[i] = new Champion;
      *p1champs[i] = *columns[0][i];
    }
    for(size_t i = 0; i < columns[1].champs_size(); i++){
      p2champs[i] = new Champion;
      *p2champs[i] = *columns[1][i];
    }
    state_manager.change_state(std::make_unique<GameState>(s,p1champs,p2champs,settings, window));
  }
}
void DraftState::dont_ban() {
  elapsed_time.restart();
  if(turns[turn_counter].is_ban_phase())
    turns[turn_counter++].do_turn(empty_champ);
}
DraftButton::DraftButton(Resources::Holder &h, const sf::String& str, [[maybe_unused]] std::function<void()> onclick_) : Button(str, std::move(onclick_)) {
  shape.setSize({150, 70});
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}
DraftState::DraftState(StateManager &state_manager, Settings& settings, sf::RenderWindow& window) : State(state_manager) {
  try{
    IOParser::File input(settings.get_champs_filepath());
    for (std::string line; std::getline(input.getfile(), line);) {
      all_champs.push_back(IOParser::create_champ(line));
    }
    if (all_champs.size() < 10) {
      throw std::invalid_argument("Not enough champions listed in the file, you need at least 10 champions to play!");
    }
  }catch(const std::invalid_argument& err){
    std::cout << err.what() << std::endl;
    state_manager.pop_state();
    return;
  }


  // create the placeholder empty champ
  empty_champ = new Champion;
  empty_champ->set_name("empty");
  // create the UI components
  sf::Vector2f window_size = StateManager::get_size(window);

  holder.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");
  buttons.push_back(new DraftButton(holder, "Lock in",
                                    [state = this, &window, &settings]()
                                    { state->lockin(state->state_manager,window,settings); }));
  buttons.push_back(new DraftButton(holder, "Don't ban", [state = this]() { state->dont_ban(); }));
  buttons.push_back(new DraftButton(holder, "back", [&state_manager](){ onclick_back(state_manager);}));

  sf::Vector2f button_size = buttons[0]->get_size();
  float margin = 5;
  UI::Grid grid{{
                    window_size.x/2 - button_size.x*static_cast<float>(buttons.size())/2,
                    window_size.y- button_size.y -margin}, {margin, margin}};
  std::vector<UI::GridElement *> els(buttons.begin(), buttons.end());
  grid.set_elements(els);
  grid.set_elements_pos();

  // creating named boxes

  sf::RectangleShape baseshape{{150, 20}};
  baseshape.setOutlineColor({33, 35, 45});
  for (size_t i = 0; i < all_champs.size(); i++) {
    champ_list.push_back(new ChampBox{all_champs[i]->get_name(), baseshape, holder, all_champs[i]});
    champ_list[i]->set_char_size(11);
    champ_list[i]->set_label_color(sf::Color::Black);
  }
  UI::Grid champgrid{{window_size.x/2 - baseshape.getSize().x, 10}, {margin, margin}, {0, 1}};
  std::vector<UI::GridElement *> champels(champ_list.begin(), champ_list.end());
  champgrid.set_elements(champels);
  champgrid.set_elements_pos();

  float col_gap = 7;
  sf::Vector2f team_col_gap_size = {150,40};
  sf::Vector2f team_col_margin = {10,10};
  std::vector<sf::Vector2f> startposes = {
      team_col_margin, {window_size.x-(team_col_gap_size.x+ team_col_margin.x), team_col_margin.x}, {team_col_margin.x, window_size.y-(team_col_gap_size.y+ team_col_margin.y) * col_gap}, {window_size.x-(team_col_gap_size.x + team_col_margin.x), window_size.y-(team_col_gap_size.y+ team_col_margin.y) * col_gap}};
  for (size_t i = 0; i < 4; i++) {
    TeamCol c{holder,startposes[i], team_col_gap_size, team_col_margin.x};
    columns.push_back(c);
    columns[i].setpos();
  }

  //initializing turns
  DraftTurn p1{columns[0].get_champs()};
  DraftTurn p2{columns[1].get_champs()};
  DraftTurn p1ban{columns[2].get_champs(),true};
  DraftTurn p2ban{columns[3].get_champs(),true};
  this->turns = std::vector<DraftTurn>{p1ban, p2ban, p1ban, p2ban, p1ban, p2ban, p1, p2, p2, p1, p1, p2, p1ban, p2ban, p1ban, p2ban, p2, p1, p1, p2};
 

  turn_counter = 0;
  elapsed_time.restart();
  selected_champ = nullptr;
  timer.setPosition({200, 40});
  timer.setFont(holder.get(Resources::Type::FONT));

  // temporarily here, so I don't have to do the draft phase while debugging
   for(size_t i = 0; i < 20; i++){
     this->turns[turn_counter++].do_turn(champ_list[i]->get_champ());
  }
  // this should be change state, but then the champions should be moved
  //state_manager.push_state(std::make_unique<GameState>(state_manager,columns[0].champs,columns[1].champs,settings, window));
}

void DraftTurn::do_turn(Champion *champ) {
  champs.push_back(champ);
}
DraftState::~DraftState() {
  for (auto & champ : all_champs)
    delete champ;
  for(auto button: buttons){
    delete button;
  }
  for(auto champ: champ_list){
    delete champ;
  }
  delete empty_champ;
}
void DraftState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (auto & i : champ_list) {
        if (i->contains(e.mouseButton.x, e.mouseButton.y)) {
          selected_champ = i->get_champ();
        }
    }
    for (UI::Button *b : buttons) {
      b->onclick_here(e);
    }
  }
}
void DraftState::update() {
  std::string s = "Time: ";
  int waiting_time = 30;
  s += std::to_string(waiting_time - (int)elapsed_time.getElapsedTime().asSeconds());
  timer.setString(s);
  if ((int)elapsed_time.getElapsedTime().asSeconds() == waiting_time) {
    if(selected_champ != nullptr){
      turns[turn_counter++].do_turn(selected_champ);
    }else{
      if(turns[turn_counter].is_ban_phase()){
        turns[turn_counter++].do_turn(empty_champ);
      }else{
        state_manager.pop_state();
      }
    }
    elapsed_time.restart();
  }
  for (auto & i : champ_list) {
    i->set_label_color(sf::Color::Black);
  }
  if(champ_list.empty()){
    state_manager.pop_state();
  }
}
void DraftState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (auto & button : buttons) {
    button->draw_to_window(window);
  }
  for (auto & column : columns) {
    column.draw_to_window(window);
  }
  for (auto & i : champ_list) {
    if (selected_champ == i->get_champ()) {
      i->set_label_color({100, 100, 100});
    }
    i->draw(window);
  }
  window.draw(timer);
}

void TeamCol::setpos() {
  sf::Vector2f pos = startpos;
  if (!elements.empty()) {
    elements[0].set_position(pos);
    pos.y += elements[0].get_size().y + margin;
  }
  for (auto & element : elements) {
    element.set_position(pos);
    pos.y += element.get_size().y + margin;
  }
}

TeamCol::TeamCol(Resources::Holder &h, sf::Vector2f startpos,sf::Vector2f size, float margin) {
  this->startpos = startpos;
  this->margin = margin;
  for (size_t i = 0; i < 5; i++) {
    elements.emplace_back(h,size);
    elements[i].set_char_size(12);
  }
}

void TeamCol::draw_to_window(sf::RenderWindow &w) {
  for (size_t i = 0; i < elements.size(); i++) {
    if (i < champs.size()) {
      elements[i].set_label(champs[i]->get_name());
    }
    elements[i].draw(w);
  }
}
