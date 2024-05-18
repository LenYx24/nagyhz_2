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
  sf::Vector2f window_size_f = {static_cast<float>(window_size.x),static_cast<float>(window_size.y)};
  textboxes.push_back(new UI::TextBox{"champions file:",resources_holder,{window_size_f.x/2,300}});
  textboxes.push_back(new UI::TextBox{"items file:",resources_holder,{window_size_f.x/2,360}});
}
void MainState::handle_events(sf::Event &event){
  if (event.type == sf::Event::Closed) {
    state_manager.exit();
  }
  else if (event.type == sf::Event::MouseButtonPressed) {
    for (UI::TextBox* t : textboxes) {
      if (t->contains(event.mouseButton.x,event.mouseButton.y)) {
        for (UI::TextBox* t1 : textboxes) {t1->set_selected(false);}
        t->set_selected(true);
        return;
      }
    }
    // if the user clicked outside of the textboxes, then unselected them
    for (UI::TextBox* t : textboxes) {t->set_selected(false);}
    for (UI::Button* b : buttons) {
      if (b->contains(event.mouseButton.x,event.mouseButton.y)) {
        b->onclick();
      }
    }
  }
  else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace){
    for(UI::TextBox *textbox : textboxes){
      if(textbox->get_is_selected()){
        std::cout << "remove char" << std::endl;
        textbox->remove_char();
        return;
      }
    }
  }
  else if(event.type == sf::Event::TextEntered){
    for(UI::TextBox *textbox : textboxes){
      if(textbox->get_is_selected()){
        char c = static_cast<char>(event.text.unicode);
        std::cout << "char : " << c << std::endl;
        textbox->add_char(c);
      }
    }
  }
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
void MainState::draw(sf::RenderWindow& window){
    MenuState::draw(window);
    for(UI::TextBox *textbox : textboxes){
      textbox->draw(window);
    }
}
MenuState::~MenuState(){
  for(auto & button : buttons){
    delete button;
  }
}
MainState::~MainState(){
  for(auto & textbox : textboxes){
    delete textbox;
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
