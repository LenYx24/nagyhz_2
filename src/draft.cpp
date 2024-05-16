#include "../include/draft.hpp"
void onclick_back(StateManager &s) {
  s.pop_state();
}

void DraftState::lockin(StateManager &s, sf::RenderWindow& window) {
  if (selectedchamp != nullptr) {
    elapsedtime.restart();
    turns[turn_counter++].doturn(selectedchamp);
    std::vector<ChampBox*>::iterator it = champlist.begin();
    for(;it != champlist.end();it++){
      if((*it)->champ->getname() == selectedchamp->getname()){
        champlist.erase(it);
        break;
      }
    }
    selectedchamp = nullptr;
  }
  if(turn_counter == 20){
    GameMode m = GameMode::THEMSELVES;
    // this should be change state, but then the champions should be moved
    state_manager.push_state(std::make_unique<GameState>(s,columns[0].champs,columns[1].champs,m, window));
  }
}
void DraftState::dontban(StateManager &s) {
  elapsedtime.restart();
  // todo: check if its banphase or not, and only do that
  if(turns[turn_counter].isbanphase())
    turns[turn_counter++].doturn(emptychamp);
}
DraftButton::DraftButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}
DraftState::DraftState(StateManager &state_manager, const Settings s, sf::RenderWindow& window) : State(state_manager) {
  // load champions from file, check if its valid, if not then close the game
  iofile inp("examples/champions.txt");
  for (std::string line; std::getline(inp.getfile(), line);) {
    Champion *c = new Champion;
    c->readfromstring(line);
    allchamps.push_back(c);
  }
  if (allchamps.size() < 10) {
    throw "Not enough champions listed in the file, you need at least 10 champions to play!";
  }
  // create the placeholder empty champ
  emptychamp = new Champion;
  emptychamp->setname("empty");
  // create the UI components
  sf::Vector2f windowsize = state_manager.get_size(window);

  h.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");
  buttons.push_back(new DraftButton(h, "Lock in", [state = this, &window](StateManager &s) { state->lockin(s,window); }));
  buttons.push_back(new DraftButton(h, "Don't ban", [state = this](StateManager &s) { state->dontban(s); }));
  buttons.push_back(new DraftButton(h, "back", onclick_back));
  sf::Vector2f buttonsize = buttons[0]->get_size();
  float margin = 5;
  UI::Grid grid{{windowsize.x/2 -buttonsize.x*buttons.size()/2, windowsize.y-buttonsize.y -margin}, {margin, margin}};
  std::vector<UI::GridElement *> els(buttons.begin(), buttons.end());
  grid.setelements(els);
  grid.setelementspos();
  // creating named boxes

  sf::RectangleShape baseshape{{150, 30}};
  baseshape.setOutlineColor({33, 35, 45});
  for (size_t i = 0; i < allchamps.size(); i++) {
    champlist.push_back(new ChampBox{allchamps[i]->getname(), baseshape, h, allchamps[i]});
    champlist[i]->setcharsize(11);
    champlist[i]->setlabelcolor(sf::Color::Black);
  }
  UI::Grid champgrid{{windowsize.x/2 - baseshape.getSize().x, 10}, {margin, margin}, {0, 1}};
  std::vector<UI::GridElement *> champels(champlist.begin(), champlist.end());
  champgrid.setelements(champels);
  champgrid.setelementspos();

  int colgap = 7;
  sf::Vector2f teamcolrectsize = {150,40};
  sf::Vector2f teamcol_margin = {10,10};
  std::vector<sf::Vector2f> startposes = {teamcol_margin, {windowsize.x-teamcolrectsize.x, teamcol_margin.x}, {teamcol_margin.x, windowsize.y-(teamcolrectsize.y+teamcol_margin.y) * colgap }, {windowsize.x-teamcolrectsize.x, windowsize.y-(teamcolrectsize.y+teamcol_margin.y) * colgap}};
  for (size_t i = 0; i < 4; i++) {
    TeamCol c{h,startposes[i],teamcolrectsize,static_cast<int>(teamcol_margin.x)};
    columns.push_back(c);
    columns[i].setpos();
  }
  DraftTurn p1{columns[0].champs};
  DraftTurn p2{columns[1].champs};
  DraftTurn p1ban{columns[2].champs,true};
  DraftTurn p2ban{columns[3].champs,true};
  this->turns = std::vector<DraftTurn>{p1ban, p2ban, p1ban, p2ban, p1ban, p2ban, p1, p2, p2, p1, p1, p2, p1ban, p2ban, p1ban, p2ban, p2, p1, p1, p2};
 

  turn_counter = 0;
  elapsedtime.restart();
  selectedchamp = nullptr;
  timer.setPosition({200, 40});
  timer.setFont(h.get(Resources::Type::FONT));

  // temporarily here, so i dont have to do the draft phase while debugging
   for(size_t i = 0; i < 20; i++){
    this->turns[turn_counter++].doturn(champlist[i]->champ);
  }
  GameMode m = GameMode::THEMSELVES;
  // this should be change state, but then the champions should be moved
  state_manager.push_state(std::make_unique<GameState>(state_manager,columns[0].champs,columns[1].champs,m, window));
}

void DraftTurn::doturn(Champion *c) {
  champs.push_back(c);
}
DraftState::~DraftState() {
  for(size_t i = 0; i < champlist.size(); i++){
    delete champlist[i];
  }
}
void DraftState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    std::cout << "[[INFO]] mouse clicked" << std::endl;
    for (size_t i = 0; i < champlist.size(); i++) {
        if (champlist[i]->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
          selectedchamp = champlist[i]->champ;
        }
    }
    for (UI::Button *b : buttons) {
      if (b->get_global_bounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(state_manager);
      }
    }
  }
}
void DraftState::update() {
  std::string s = "Time: ";
  int waiting_time = 30;
  s += std::to_string(waiting_time - (int)elapsedtime.getElapsedTime().asSeconds());
  timer.setString(s);
  if ((int)elapsedtime.getElapsedTime().asSeconds() == waiting_time) {
    if(selectedchamp != nullptr){
      turns[turn_counter++].doturn(selectedchamp);
    }else{
      if(turns[turn_counter].isbanphase()){
        turns[turn_counter++].doturn(emptychamp);
      }else{
        state_manager.pop_state();
      }
    }
    elapsedtime.restart();
  }
  for (size_t i = 0; i < champlist.size(); i++) {
    champlist[i]->setlabelcolor(sf::Color::Black);
  }
  if(champlist.size() == 0){
    state_manager.pop_state();
  }
}
void DraftState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->draw_to_window(window);
  }
  for (size_t i = 0; i < columns.size(); i++) {
    columns[i].draw_to_window(window);
  }
  for (size_t i = 0; i < champlist.size(); i++) {
    if (selectedchamp == champlist[i]->champ) {
      champlist[i]->setlabelcolor({100, 100, 100});
    }
    champlist[i]->draw(window);
  }
  window.draw(timer);
}

void TeamCol::setpos() {
  sf::Vector2f pos = startpos;
  if (!elements.empty()) {
    elements[0].set_position(pos);
    pos.y += elements[0].get_size().y + margin;
  }
  for (size_t i = 0; i < elements.size(); i++) {
    elements[i].set_position(pos);
    pos.y += elements[i].get_size().y + margin;
  }
}

TeamCol::TeamCol(Resources::Holder &h, sf::Vector2f startpos,sf::Vector2f size, int margin) {
  this->startpos = startpos;
  this->margin = margin;
  for (size_t i = 0; i < 5; i++) {
    elements.push_back(DraftNamedBox{h, "",size});
    elements[i].setcharsize(12);
  }
}

void TeamCol::draw_to_window(sf::RenderWindow &w) {
  for (size_t i = 0; i < elements.size(); i++) {
    if (i < champs.size()) {
      elements[i].setlabel(champs[i]->getname());
    }
    elements[i].draw(w);
  }
}
