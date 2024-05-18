#include "../include/gamemoves.hpp"
#include "../include/map.hpp"
#include "../include/gameobjects.hpp"
bool GameMove::basic_check( Player *current_player, Champion *selected_champ){
    return selected_champ != nullptr && current_player->ishischamp(selected_champ) && !current_player->is_gamemove_active() && selected_champ->getmovepoints() - points >= 0;
}
int GameMove::points = 1;
void MoveCell::onclick(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ){
    if(GameMove::basic_check(current_player,selected_champ)){
        map->select_accessible_cells(selected_champ);
        selected_champ->add_gamemove(this);
    }
}
void TeleportBase::onclick(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ){
    if(GameMove::basic_check(current_player,selected_champ)){
        selected_champ->add_gamemove(this);
        map->update_vision();
    }
}
void PlaceWard::onclick(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ){
    if(GameMove::basic_check(current_player,selected_champ)){
        map->select_wardable_cells(selected_champ);
        selected_champ->add_gamemove(this);
    }
}
// maybe need a couple different attacks:
// against champs
// against minions
// against every other standing objective
void AttackMove::onclick(std::shared_ptr<Map> map, Player *current_player, Champion *selected_champ){
    if(GameMove::basic_check(current_player,selected_champ)){
        map->select_accessible_cells(selected_champ);
        selected_champ->add_gamemove(this);
    }
}
void MoveCell::do_move(Champion *champ, std::shared_ptr<Map> map){
    map->move(champ,champ->get_real_cell()->getindex(),cell->getindex());
    champ->setcell(cell);
}
void TeleportBase::do_move(Champion *champ, std::shared_ptr<Map> map){
    map->move(champ,champ->get_real_cell()->getindex(),cell->getindex());
    champ->setcell(cell);
    champ->refill_hp();
}
void PlaceWard::do_move(Champion *champ, std::shared_ptr<Map> map){
    champ->place_ward(map,cell);
    map->update_vision();
}
void AttackMove::do_move(Champion *champ, std::shared_ptr<Map> map){
    map->move(champ,champ->get_real_cell()->getindex(),cell->getindex());
    champ->setcell(cell);
    // Todo: if there are no enemy entity to attack, then cancel move
    Entity *other = cell->get_first_entity();
    if(champ->get_side() != other->get_side())
      champ->fight(other);
}