#include "../include/gamemoves.hpp"
#include "../include/map.hpp"

bool GameMove::check_gamemove_addable( Player *current_player, Champion *selected_champ)const{
    return selected_champ != nullptr && current_player->is_his_champ(selected_champ) && !current_player->is_gamemove_active() && selected_champ->getmovepoints() - points >= 0;
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