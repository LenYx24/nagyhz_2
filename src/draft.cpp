#include "../include/draft.hpp"
void onclick_back(StateManager &s) {
  s.PopState();
}
void onclick_lockin(StateManager &s) {
  s.PopState();
}
DraftButton::DraftButton(Resources::Holder &h, sf::String str, std::function<void(StateManager &s)> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
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
  // Todo: put this into its own grid class
  float marginy = 20 + buttons[0].getSize().y / 2.f;
  sf::Vector2f pos{800 / 2.f, marginy};
  for (size_t i = 0; i < buttons.size(); i++) {
    buttons[i].setPosition(pos);
    pos.y += buttons[i].getSize().y / 2.f + marginy;
  }
  // draft phase turns
  DraftTurn p1ban{p1banchamps};
  DraftTurn p2ban{p1banchamps};
  DraftTurn p1{p1banchamps};
  DraftTurn p2{p1banchamps};
  // Todo: clean this up (but the advantage of giving it like this, is it's easy to change)
  std::vector<DraftTurn> turns = {p1ban, p2ban, p1ban, p2ban, p1ban, p2ban, p1, p2, p2, p1, p1, p2, p1ban, p2ban, p1ban, p2ban, p2, p1, p1, p2};
  elapsedtime = sf::Time{sf::seconds(30.f)};
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
  elapsedtime -= sf::Time{sf::seconds(1)};
  if (elapsedtime == sf::Time::Zero) {
    // do turn move
  }
  w.display();
}
void DraftTurn::doturn(Champion *c) {
  champs.push_back(c);
}
