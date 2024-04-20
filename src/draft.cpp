#include "../include/draft.hpp"

DraftState::DraftState(const GameMode m) {
  // load champions from file, check if its valid, if not then close the game
  //
  // create the UI components
}
// onclicks:
// select champ -> shows the champ details on the small grid
// lock in -> sets current champ pointer to null, removes champ from listofchamps
//  adds it to the
// dont ban -> sets current champ pointer to null, ads to the ban list, and moves phases
// back button -> cleans up after himself (should be automatic) and pops the current state
void HandleEvents(StateManager &s, Renderer &renderer) {
  // check if user clicked on an element, then do the task accordingly
}
void Update(StateManager &s, Renderer &r) {
  // show ui components
  // check if its ban phase currently, only draw the ban button then, or make the ban button not do anything while its not banphase
  // update time elapsed
}
