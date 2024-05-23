#include "../include/gamemoves.hpp"
#include "../include/map.hpp"

bool GameMove::check_gamemove_addable( Player *current_player, Champion *selected_champ){
    return selected_champ != nullptr
         && current_player->is_his_champ(selected_champ)
         && !current_player->is_gamemove_active()
         && selected_champ->getmovepoints() - points >= 0;
}
std::string GameMove::get_state_info()const{
  if(cell == nullptr)return ";not_complete\n";
  sf::Vector2f index = cell->get_index();
  std::string info = "cell: x:"+std::to_string(index.x) + " y:"+std::to_string(index.y)+";";
  return info;
}
std::string GameMove::get_formatted_info(const std::string& name)const{
  std::string info = GameMove::get_state_info();
  info += "type:"+name+";";
  return info;
}
std::string MoveCell::get_state_info()const{
  return get_formatted_info("movecell");
}
std::string TeleportBase::get_state_info()const{
  return get_formatted_info("teleportbase");
}
std::string AttackMove::get_state_info()const{
  return get_formatted_info("attackmove");
}
std::string PlaceWard::get_state_info()const{
  return get_formatted_info("placeward");
}

void MoveCell::do_move(Champion *champ, std::shared_ptr<Map> map){
    map->move(champ, champ->get_real_cell()->get_index(), cell->get_index());
    champ->set_cell(cell);
}
void TeleportBase::do_move(Champion *champ, std::shared_ptr<Map> map){
    map->move(champ, champ->get_real_cell()->get_index(), cell->get_index());
    champ->set_cell(cell);
    champ->refill_hp();
}
void PlaceWard::do_move(Champion *champ, std::shared_ptr<Map> map){
    champ->place_ward(map,cell);
    map->update_vision();
}
void AttackMove::do_move(Champion *champ, std::shared_ptr<Map> map){
    map->check_game_end();
    if(!cell)throw std::runtime_error("gamemove was not finished");
    Entity *other = cell->get_first_entity();
    if(champ->get_side() != other->get_side())
      champ->fight(other);
}