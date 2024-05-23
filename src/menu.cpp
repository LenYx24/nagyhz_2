#include <utility>

#include "../include/menu.hpp"

namespace Menu {
MainState::MainState(StateManager &state_manager, sf::RenderWindow &window, Settings setting) :
      MenuState(state_manager,setting) {
  resources_holder.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");

  std::function<void()> onclick_start = [state=this,&window, &state_manager](){
    state->setting.set_champs_filepath(state->text_boxes["champs"]->get_text());
    state->setting.set_items_filepath(state->text_boxes["items"]->get_text());
    state->setting.set_output_prefix(state->text_boxes["output"]->get_text());
    state_manager.push_state(std::make_unique<ModeSelectionState>(state_manager,window,state->setting));
  };
  buttons.push_back(new MenuButton{resources_holder, "Start", onclick_start});
  buttons.push_back(new MenuButton{resources_holder, "Exit", [state=this](){state->state_manager.pop_state();}});
  sf::Vector2u window_size = window.getSize();
  UI::Grid grid{{static_cast<float>(window_size.x) / 2.f, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> els(buttons.begin(), buttons.end());
  grid.set_elements(els);
  grid.set_elements_pos();
  sf::Vector2f window_size_f = {static_cast<float>(window_size.x),static_cast<float>(window_size.y)};
  text_boxes["champs"] = (new UI::TextBox{"champions file:",
                                          resources_holder,
                                          {window_size_f.x/2,300},
                                          setting.get_champs_filepath()});
  text_boxes["items"] = (new UI::TextBox{"items file:",
                                         resources_holder,
                                         {window_size_f.x/2,360},
                                         setting.get_items_filepath()});
  text_boxes["output"] = (new UI::TextBox{"game output prefix:",
                                          resources_holder,
                                          {window_size_f.x/2,420},
                                          setting.get_output_prefix()});
}
void MainState::handle_events(sf::Event &event){
  if (event.type == sf::Event::Closed) {
    state_manager.exit();
  }
  else if (event.type == sf::Event::MouseButtonPressed) {
    bool clicked_inside_textbox = false;
    for (auto t : text_boxes) {
      if (t.second->contains(event.mouseButton.x,event.mouseButton.y)) {
        clicked_inside_textbox = true;
        for (auto t1 : text_boxes) {t1.second->set_selected(false);}
        t.second->set_selected(true);
      }
    }
    // if the user clicked outside the text_boxes, then unselected them
    if(!clicked_inside_textbox)
      for(auto t: text_boxes) {t.second->set_selected(false);}
    for (UI::Button* b : buttons) {
      b->onclick_here(event);
    }
  }
  else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::BackSpace){
    for(auto textbox : text_boxes){
      if(textbox.second->get_is_selected()){
        textbox.second->remove_char();
        return;
      }
    }
  }
  else if(event.type == sf::Event::TextEntered && event.text.unicode != '\b'){
    for(auto textbox : text_boxes){
      if(textbox.second->get_is_selected()){
        char c = static_cast<char>(event.text.unicode);
        textbox.second->add_char(c);
      }
    }
  }
}
MenuButton::MenuButton(Resources::Holder &h, const sf::String& str, [[maybe_unused]]std::function<void()> onclick_)
    : Button(str, std::move(onclick_)) {
  // menu button specific override settings
  text.setCharacterSize(15);
  text.setFont(h.get(Resources::Type::FONT));
}


void MenuState::handle_events(sf::Event &event) {
  if (event.type == sf::Event::Closed) {
    state_manager.exit();
  } else if (event.type == sf::Event::MouseButtonPressed) {
    for (UI::Button* b : buttons) {
      b->onclick_here(event);
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
    for(auto textbox : text_boxes){
      textbox.second->draw(window);
    }
}
MenuState::~MenuState(){
  for(auto & button : buttons){
    delete button;
  }
}
MainState::~MainState(){
  for(auto & textbox : text_boxes){
    delete textbox.second;
  }
}
ModeSelectionState::ModeSelectionState(StateManager &state_manager, sf::RenderWindow& window, Settings setting)
    : MenuState(state_manager,std::move(setting)) {
  resources_holder.load(Resources::Type::FONT, "./resources/fonts/Roboto.ttf");

  std::function<void()> onclick_draft = [&window, state = this](){
    std::cout << "output prefix: " << state->setting.get_output_prefix() << std::endl;
    // Todo: catch here
    state->state_manager.push_state(std::make_unique<DraftState>(state->state_manager, state->setting, window));
  };

  // for now there's _manager only game mode, but in the future there could be more
  buttons.push_back(new MenuButton{resources_holder, "Player vs Player",
                                   [state=this,onclick_draft]()
                                   {state->setting.set_gamemode(GameMode::TWO_PLAYER);onclick_draft();}});
  buttons.push_back(new MenuButton{resources_holder, "back",
                                   [state=this]()
                                   {state->state_manager.pop_state();}});

  sf::Vector2u window_size = window.getSize();
  UI::Grid grid{{static_cast<float>(window_size.x) / 2.f, 100}, {5, 5},{0,1}};
  std::vector<UI::GridElement *> elements(buttons.begin(), buttons.end());
  grid.set_elements(elements);
  grid.set_elements_pos();
}
} // namespace Menu
