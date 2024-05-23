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
      StateManager m;
      Menu::MenuState state{m};
      sf::Event e;
      state.handle_events(e);
    }END
}