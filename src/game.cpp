#include "../include/game.hpp"
GameButton::GameButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_, sf::Vector2f pos) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(14);
  text.setFont(h.get(Resources::Type::FONT));
  this->setposition(pos);
}
GameState::GameState(StateManager &state_manager, std::vector<Champion*> p1champs,std::vector<Champion*> p2champs, GameMode mode) : State(state_manager),map(std::make_unique<Map>(sf::Vector2f{500,20})) {
  // load items from the file and save them to the allitems variable
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
  // create the UI components:
  sf::Vector2f windowsize = state_manager.get_size();
  // todo: get the gamebuttons sizes, and position it accordingly
  buttons.push_back(new GameButton(h, "End round", [state = this](StateManager &s) { state->onclick_gamemove(); },{80,40}));
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
  itemslabel->setposition({windowsize.x - itemslabel->getglobalbounds().width,20});
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
  // spawn champs
  players[0]->setchampicons("ABCDE");
  players[1]->setchampicons("FGHIJ");
  players[0]->setfont(h);
  players[1]->setfont(h);
  players[0]->setspawnpoint({0,map->getcellgridsize().y-1});
  players[1]->setspawnpoint({map->getcellgridsize().x-1,0});
  players[0]->spawnchamps(map);
  players[1]->spawnchamps(map);
  // spawn minions
  
  // seed the random generator
  srand(static_cast<unsigned>(time(0)));
  // Todo: randomly select starter champ
  currentplayer = players[0];
  selectedchamp = nullptr;
}
// onclicks:
void onclick_endturn() {
  // ends the turn and starts the other players turn, or does the simulation
  // a simulation substate needs the map, and the entities on the map
  // it also needs to know, who's able to see the moves
}
void GameState::onclick_gamemove() {
  currentplayer->setgamemoveactive(true);
  if(currentplayer->ishischamp(selectedchamp)){
    map->setselectednearbycells(selectedchamp);
  }
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
void GameState::onclick_item() {
  // check if there's currently a selected champion
  if(currentplayer->ishischamp(selectedchamp)){
    // tries to add the item to the champion (the checks are made by the champion class)
  }
}

void GameState::onclick_attack(){

}
void GameState::onclick_base(){

}
void GameState::onclick_ward(){
  
}
void Round::roundend(){}
void GameState::HandleEvents(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    _state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (GameButton *b : buttons) {
      if (b->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(_state_manager);
        return;
      }
    }
    for (GameButton *b : gamemovebuttons) {
      if (b->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(_state_manager);
        return;
      }
    }
    Cell *clickedcell = map->getclickedcell(e.mouseButton.x,e.mouseButton.y);
    if(clickedcell != nullptr){
      if(currentplayer->isgamemoveactive()){
        //add the current cell and add the gamemove to the currently selected champion
        //currentplayer->addmove();
      }
      else{
        std::cout << "clicked on cell" << std::endl;
        map->resetcolors();
        if(selectedchamp != nullptr && selectedchamp->getcell() != nullptr){
          selectedchamp->getcell()->resetcolor();
        }
        clickedcell->sethighlighted();
        sf::Vector2f index = clickedcell->getindex();
        sf::RectangleShape shape{{100,60}};
        shape.setFillColor(sf::Color::Black);
        statlabels.clear();
        UI::NamedBox *statlabel = new UI::NamedBox{"Cell:\nx: "+std::to_string((int)index.x+1) + "\ny: "+std::to_string((int)index.y+1),shape,h};
        statlabel->setcharsize(12);
        statlabel->setposition({70,200});
        statlabels.push_back(statlabel);
        Entity *clickedentity = clickedcell->getentitiyclicked(e.mouseButton.x, e.mouseButton.y);
        if(clickedentity != nullptr){
          selectedchamp = currentplayer->getselectedchamp(clickedcell->getindex());
          std::vector<std::string> statsentity = clickedentity->getstats();
          sf::Vector2f startpos{70,300};
          for(size_t i = 0; i < statsentity.size(); i++){
            UI::NamedBox *statlabel = new UI::NamedBox{statsentity[i],shape,h};
            statlabel->setposition(startpos);
            int marginy = 20;
            startpos.y += shape.getSize().y + marginy;
            statlabel->setcharsize(12);
            statlabels.push_back(statlabel);
          }
        }
      }
    }
  }
}
void GameState::Update() {
  // update time elapsed
  std::string s = "Time left: ";
  s += std::to_string(30 - (int)elapsedtime.getElapsedTime().asSeconds());
  timer.setString(s);
  if ((int)elapsedtime.getElapsedTime().asSeconds() == 30) {
    elapsedtime.restart();
    // end turn
  }
}
void GameState::Draw() {
  sf::Color background_color = sf::Color(220, 225, 222);
  sf::RenderWindow &window = _state_manager.getwindow();
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