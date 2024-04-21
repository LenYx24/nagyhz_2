#include "../include/draft.hpp"

DraftState::DraftState(const Settings s) {
  // load champions from file, check if its valid, if not then close the game
  std::list<Champion> champs;
  iofile inp("example/champs.txt");
  for (std::string line; std::getline(inp.getfile(), line);) {
    Champion c;
    c.readfromstring(line);
    champs.push_back(c);
  }
  if (champs.size() < 10) {
    throw "Not enough champions listed in the file, you need at least 10 champions to play!";
  }
  // create the UI components
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
}
