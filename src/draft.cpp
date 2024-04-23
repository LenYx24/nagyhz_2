#include "../include/draft.hpp"
void onclick_back(StateManager &s) {
  s.PopState();
}
void onclick_lockin(StateManager &s) {
  s.PopState();
}
DraftButton::DraftButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  this->setSize({200, 100});
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}
DraftState::DraftState(const Settings s) {
  // load champions from file, check if its valid, if not then close the game
  std::list<Champion> champs;
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
  buttons.push_back(DraftButton{h, "Lock in", onclick_lockin});
  buttons.push_back(DraftButton{h, "Not banning"});
  buttons.push_back(DraftButton{h, "back", onclick_back});

  float marginx = 30;
  std::cout << "marginx: " << marginx << std::endl;
  sf::Vector2f pos{buttons[0].getSize().x + marginx, 400};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].setPosition(pos);
    pos.x += buttons[i].getSize().x + marginx;
  }
  // creating the namedboxes
  std::vector<sf::Vector2f> startposes = {{20, 20}, {700, 20}, {20, 300}, {700, 300}};
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
}
// onclicks:
// select champ -> shows the champ details on the small grid
// lock in -> sets current champ pointer to null, removes champ from listofchamps
//  adds it to the
// dont ban -> sets current champ pointer to null, ads to the ban list, and moves phases
// back button -> cleans up after himself (should be automatic) and pops the current state
void DraftState::HandleEvents(StateManager &s, Renderer &renderer) {
  // check if user clicked on an element, then do the task accordingly
  for (sf::Event event{}; renderer.PollEvent(event);) {
    if (event.type == sf::Event::Closed) {
      s.exit();
    } else if (event.type == sf::Event::MouseButtonPressed) {
      std::cout << "[[INFO]] mouse clicked" << std::endl;
      for (Button b : buttons) {
        if (b.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
          b.onclick(s);
        }
      }
    }
  }
}
void DraftTurn::doturn(Champion *c) {
  if (c == nullptr) {
    throw "nullptr";
  }
  champs.push_back(c);
}
void DraftState::Update(StateManager &s, Renderer &r) {
  // show ui components
  // check if its ban phase currently, only draw the ban button then, or make the ban button not do anything while its not banphase
  // update time elapsed
  // if elapsed time reaches a certain point, then act accordingly, either go back to menu, or ban nothing
  sf::RenderWindow &w = r.getWindow();
  sf::Color background_color = sf::Color(220, 225, 222);
  w.clear(background_color);
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].draw_to_window(w);
  }
  for (size_t i = 0; i < columns.size(); i++) {
    columns[i].draw_to_window(w);
  }
  std::cout << "time: " << elapsedtime.getElapsedTime().asSeconds() << std::endl;
  if (elapsedtime.getElapsedTime().asSeconds() == 30) {
    // do turn move
    // turns[turn_counter++].doturn(selectedchamp);
    elapsedtime.restart();
  }
  w.display();
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
    elements.push_back(DraftNamedBox{});
  }
}

void TeamCol::draw_to_window(sf::RenderWindow &w) {
  for (size_t i = 0; i < elements.size(); i++) {
    elements[i].draw(w);
  }
}
