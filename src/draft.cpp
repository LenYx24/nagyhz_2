#include "../include/draft.hpp"
void onclick_back(StateManager &s) {
  s.PopState();
}
void onclick_lockin(StateManager &s) {
  // lockin
}
DraftButton::DraftButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  shape.setSize({150, 70});
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}
DraftState::DraftState(StateManager &state_manager, const Settings s) : State(state_manager) {
  // load champions from file, check if its valid, if not then close the game
  std::vector<Champion> champs;
  iofile inp("examples/champions.txt");
  for (std::string line; std::getline(inp.getfile(), line);) {
    Champion c;
    c.readfromstring(line);
    champs.push_back(c);
  }
  if (champs.size() < 10) {
    throw "Not enough champions listed in the file, you need at least 10 champions to play!";
  }
  // create the UI components

  h.load(Resources::Type::FONT, "./fonts/Roboto.ttf");
  buttons.push_back(new DraftButton(h, "Lock in", onclick_lockin));
  buttons.push_back(new DraftButton(h, "Don't ban", onclick_lockin));
  buttons.push_back(new DraftButton(h, "back", onclick_back));

  UI::Grid grid{{250, 500}, {5, 5}};
  std::vector<UI::GridElement *> els(buttons.begin(), buttons.end());
  grid.setelements(els);
  grid.setelementspos();
  // creating named boxes
  sf::RectangleShape baseshape{{150, 30}};
  baseshape.setOutlineColor({33, 35, 45});
  for (size_t i = 0; i < champs.size(); i++) {
    champlist.push_back(new ChampBox{champs[i].getname(), baseshape, h, &champs[i]});
    champlist[i]->setcharsize(11);
    champlist[i]->setlabelcolor(sf::Color::Black);
  }
  UI::Grid champgrid{{300, 10}, {5, 5}, {0, 1}};
  std::vector<UI::GridElement *> champels(champlist.begin(), champlist.end());
  champgrid.setelements(champels);
  champgrid.setelementspos();

  std::vector<sf::Vector2f> startposes = {{20, 5}, {650, 5}, {20, 300}, {650, 300}};
  for (size_t i = 0; i < 4; i++) {
    TeamCol c{startposes[i]};
    columns.push_back(c);
    columns[i].setpos();
  }
  DraftTurn p1{columns[0].champs};
  DraftTurn p2{columns[1].champs};
  DraftTurn p1ban{columns[2].champs};
  DraftTurn p2ban{columns[3].champs};
  // Todo: clean this up (but the advantage of giving it like this, is it's easy to change)
  std::vector<DraftTurn> turns = {p1ban, p2ban, p1ban, p2ban, p1ban, p2ban, p1, p2, p2, p1, p1, p2, p1ban, p2ban, p1ban, p2ban, p2, p1, p1, p2};
  turn_counter = 0;
  elapsedtime.restart();
  selectedchamp = nullptr;
  timer.setPosition({150, 150});
  timer.setFont(h.get(Resources::Type::FONT));
}
// onclicks:
// select champ -> shows the champ details on the small grid
// lock in -> sets current champ pointer to null, removes champ from listofchamps
//  adds it to the
// dont ban -> sets current champ pointer to null, ads to the ban list, and moves phases
// back button -> cleans up after himself (should be automatic) and pops the current state

void DraftTurn::doturn(Champion *c) {
  if (c == nullptr) {
    throw "nullptr";
  }
  champs.push_back(c);
}
DraftState::~DraftState() {
  for (size_t i = 0; i < champlist.size(); i++) {
    delete champlist[i];
  }
}
void DraftState::HandleEvents(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    _state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    std::cout << "[[INFO]] mouse clicked" << std::endl;
    for (UI::Button *b : buttons) {
      if (b->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        b->onclick(_state_manager);
      }
    }
    for (ChampBox *b : champlist) {
      if (b->getglobalbounds().contains(e.mouseButton.x, e.mouseButton.y)) {
        selectedchamp = b->champ;
      }
    }
  }
}
void DraftState::Update() {
  // show ui components
  // check if its ban phase currently, only draw the ban button then, or make the ban button not do anything while its not banphase
  // update time elapsed
  // if elapsed time reaches a certain point, then act accordingly, either go back to menu, or ban nothing

  std::string s = "Ido: ";
  s += std::to_string(30 - (int)elapsedtime.getElapsedTime().asSeconds());
  timer.setString(s);
  if (elapsedtime.getElapsedTime().asSeconds() == 30) {
    // do turn move
    // turns[turn_counter++].doturn(selectedchamp);
    elapsedtime.restart();
  }
}
void DraftState::Draw(sf::RenderWindow &window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i]->draw_to_window(window);
  }
  for (size_t i = 0; i < columns.size(); i++) {
    columns[i].draw_to_window(window);
  }
  for (size_t i = 0; i < champlist.size(); i++) {
    champlist[i]->draw(window);
  }
  window.draw(timer);
}

void TeamCol::setpos() {
  sf::Vector2f pos = startpos;
  if (!elements.empty()) {
    elements[0].setposition(pos);
    pos.y += elements[0].getsize().y + margin;
  }
  for (size_t i = 0; i < elements.size(); i++) {
    elements[i].setposition(pos);
    pos.y += elements[i].getsize().y + margin;
  }
}

TeamCol::TeamCol(sf::Vector2f startpos, int margin) {
  this->startpos = startpos;
  this->margin = margin;
  for (size_t i = 0; i < 5; i++) {
    elements.push_back(DraftNamedBox{""});
  }
}

void TeamCol::draw_to_window(sf::RenderWindow &w) {
  for (size_t i = 0; i < elements.size(); i++) {
    elements[i].draw(w);
  }
}
