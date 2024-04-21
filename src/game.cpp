#include "../include/game.hpp"

GameState::GameState(Champion *allchamps[10], GameMode mode) {
  // load items from the file and save them to the variable
  // load font
  // create the UI components:
  // create selected champ panel, with stats and one button
  // every move should be present, and have their moves, but it checks if it is a legal move.
  // create a map and call the create map method, which creates a simple map, with the basic entities on it
  // create the items panel, and list all the items 

  // sets the current player to the first player
}
// onclicks:
void onclick_endround(){
    // ends round for one of the players
}
void onclick_gamemove(){
    // checks if a champion is selected
    // gets the current cell, where the champion is right now
    // asks the gamemove, to validate the move
}
void onclick_cell(){
    // checks if there's a selected move right now
    // if there's, then if the cell was clickable, then add the gamemove to the currently selected champions gamemove
    // reset the timer
    // reset clickable cells
}
void onclick_champ(){
    // update selected champs stats
    // update the available game moves
}
void onclick_item(){
    // check if there's currently a selected champion
    // tries to add the item to the champion
}
void GameState::HandleEvents(StateManager &s, Renderer &renderer) {
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
void GameState::Update(StateManager &s, Renderer &r) {
  // if a champion is not selected, then dont show move buttons, and items, and selected info
  // if a champ is selected, show moves, and stats
  // if a champ is selected and is on its base cell, then show items
  // show ui components

  // 
  // update time elapsed
  // if elapsed time reaches a certain point, then act accordingly => end turn
  // update the vision on a cell, by calling every entity, and call update vision on it
}
