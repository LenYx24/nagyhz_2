#include "../include/menu.hpp"

namespace Menu {
MainState::MainState(StateManager &state_manager, sf::RenderWindow &window) : MenuState(state_manager) {
  resources_holder.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");

  std::function<void()> onclick_start = [&window, &state_manager](){state_manager.push_state(std::make_unique<ModeSelectionState>(state_manager,window));};
  buttons.push_back(new MenuButton{resources_holder, "Start", onclick_start});
  buttons.push_back(new MenuButton{resources_holder, "Exit", [state=this](){state->state_manager.pop_state();}});
  sf::Vector2u window_size = window.getSize();
  UI::Grid grid{{static_cast<float>(window_size.x) / 2.f, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> els(buttons.begin(), buttons.end());
  grid.setelements(els);
  grid.setelementspos();

}
MenuButton::MenuButton(Resources::Holder &h, sf::String str, std::function<void()> onclick_) : Button(str, onclick_) {
  // menu button specific override settings
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}


void MenuState::handle_events(sf::Event &e) {
  if (e.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (e.type == sf::Event::MouseButtonPressed) {
    for (UI::Button* b : buttons) {
      if (b->contains(e.mouseButton.x,e.mouseButton.y)) {
        b->onclick();
      }
    }
  }
}
void MenuState::update() {}
void MenuState::draw(sf::RenderWindow& window) {
  sf::Color background_color = sf::Color(220, 225, 222);
  window.clear(background_color);
  for (auto & button : buttons) {
    button->draw_to_window(window);
  }
}
MenuState::~MenuState(){
  for(auto & button : buttons){
    delete button;
  }
}
ModeSelectionState::ModeSelectionState(StateManager &s, sf::RenderWindow& window) : MenuState(s) {
  resources_holder.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");

  std::function<void()> onclick_draft = [&window, state = this](){
    Settings settings{"examples/champs.txt","examples/items.txt",GameMode::TWOPLAYER};
    state->state_manager.push_state(std::make_unique<DraftState>(state->state_manager, settings, window));
  };

  // for now there's only gamemode, but in the future there could be more
  buttons.push_back(new MenuButton{resources_holder, "Player vs Player", onclick_draft});
  buttons.push_back(new MenuButton{resources_holder, "back", [state=this](){state->state_manager.pop_state();}});

  sf::Vector2u window_size = window.getSize();
  UI::Grid grid{{static_cast<float>(window_size.x) / 2.f, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> els(buttons.begin(), buttons.end());
  grid.setelements(els);
  grid.setelementspos();
}
} // namespace Menu
