#include <iostream>
#include "gtest_lite.h"
#include "../include/gameobjects.hpp"
#include "../include/ioparser.h"
#include "../include/menu.hpp"
#include <vector>

bool champexiststest(std::vector<Champion *> &champs, const std::string& name){
    for (auto & champ : champs) {
        if(champ->get_name() == name)
            return true;
    }
    return false;
}
int main(){
    // seed the random number
    srand(static_cast<unsigned>(time(nullptr)));
    TEST(champs,champ comprehensive properties check){
      IOParser::File inp("./test/champstest.txt");
      std::vector<Champion *> champs;
      EXPECT_NO_THROW(
          for (std::string line; std::getline(inp.getfile(), line);) {
            champs.push_back(IOParser::create_champ(line));
          }
      );
      EXPECT_TRUE(champexiststest(champs,"akali"));
      EXPECT_TRUE(champexiststest(champs,"alistar"));
      EXPECT_TRUE(champexiststest(champs,"braum"));
      if(!champs.empty()){
        Champion *champ = champs[0];

        EXPECT_EQ(champ->can_fight_back(),true);
        EXPECT_EQ(champ->is_alive(),true);
        EXPECT_EQ(champ->should_focus(),false);

        champ->clear_gamemoves();

        EXPECT_EQ(champ->gives_creep_score(), false);
        EXPECT_EQ(champ->get_simulation_cell(), nullptr);
        Cell *cell = new Cell;
        champ->set_cell(cell);
        EXPECT_EQ(champ->get_simulation_cell(),champ->get_real_cell());

        // frees
        delete cell;
      }
      for(size_t i = 0; i < champs.size(); i++){
        delete champs[i];
      }
    }END
    TEST(items,reading items){
      IOParser::File inp("./test/itemstest.txt");
      std::vector<Item> items;
      EXPECT_NO_THROW(
          for (std::string line; std::getline(inp.getfile(), line);) {
            Item item = IOParser::create_item(line);
            items.push_back(item);
          }
      );
      EXPECT_EQ("liandry",items[0].get_name());
      EXPECT_EQ(400,items[0].get_gold_value());
      EXPECT_EQ(20,items[0].get_bonus_dmg());
      EXPECT_EQ(30,items[0].get_bonus_hp());
    }END
    TEST(menu,testing menustate){
      sf::Window window;
      StateManager state_manager;
      Settings setting{"examples/champions.txt",
                 "examples/items.txt",
                 "examples/game",
                 GameMode::TWO_PLAYER};
      Menu::MenuState state{state_manager,setting};
      sf::Event e;
      state.handle_events(e);
    }END
    TEST(champion, champ fight){
      auto champ_1 = new Champion{"akali",100,5,200,20};
      auto champ_2 = new Champion{"zed",100,5,200,20};
      std::cout << "champ 1 current hp: " << champ_1->get_current_hp() << std::endl;
      std::cout << "champ 2 current hp: " << champ_1->get_current_hp() << std::endl;
      champ_1->fight(champ_2);
      EXPECT_EQ(champ_1->get_current_hp(),champ_2->get_current_hp());
      std::cout << "champ 1 current hp after fight: " << champ_1->get_current_hp() << std::endl;
      std::cout << "champ 2 current hp after fight: " << champ_1->get_current_hp() << std::endl;
      champ_1->fight(champ_2);
      std::cout << "the winner is the one alive, as the next fight is won by chance" << std::endl;
      std::cout << "champ_1 " << champ_1->is_alive() << std::endl;
      std::cout << "champ_2 " << champ_2->is_alive() << std::endl;
      // free
      delete champ_1;
      delete champ_2;
    }END
    TEST(champion, moving the champ){
      auto champ = new Champion{"akali",100,5,200,20};
      std::shared_ptr<Map> map = std::make_shared<Map>(sf::Vector2f{0,0});
      champ->set_cell(map->getcell({0,0}));
      Cell *champ_cell = champ->get_real_cell();
      std::cout << "champion pos: x: " << champ_cell->get_index().x
                << " y: " << champ_cell->get_index().y << std::endl;
      // create the move, finish it and add it to the champion
      auto move = new MoveCell;
      sf::Vector2 newpos_index = sf::Vector2f{1,1};
      move->finish(map->getcell(newpos_index));
      champ->add_gamemove(move);
      champ->do_move(map);

      std::cout << "champion new pos: x: " << champ->get_real_cell()->get_index().x
                << " y: " << champ->get_real_cell()->get_index().y << std::endl;

      champ_cell = champ->get_simulation_cell();
      sf::Vector2f expected_newpos_index = champ_cell->get_index();
      EXPECT_EQ(newpos_index.x,expected_newpos_index.x);
      EXPECT_EQ(newpos_index.y,expected_newpos_index.y);
      champ->move(map);

      map->de_spawn(champ);
      delete champ;
    }END
    TEST(champion, warding){
      auto champ = new Champion{"anivia",100,5,200,20};
      std::shared_ptr<Map> map = std::make_shared<Map>(sf::Vector2f{0,0});
      champ->set_cell(map->getcell({0,0}));
      // create the move, finish it and add it to the champion
      auto move = new PlaceWard;
      sf::Vector2 newpos_index = sf::Vector2f{1,1};
      move->finish(map->getcell(newpos_index));
      champ->add_gamemove(move);
      champ->do_move(map);

      map->de_spawn(champ);
      delete champ;
    }END
}