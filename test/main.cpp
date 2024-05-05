#include <iostream>
#include "gtest_lite.h"
#include "../include/gameobjects.hpp"
#include "../include/ioparser.hpp"
#include <vector>

bool champexiststest(std::vector<Champion> &champs, std::string name){
    for (size_t i = 0; i < champs.size(); i++) {
        if(champs[i].getname() == name)
            return true;
    }
    return false;
}
int main(){
    iofile inp("./champstest.txt");
    std::vector<Champion> champs;
    EXPECT_NO_THROW(
        for (std::string line; std::getline(inp.getfile(), line);) {
            Champion c;
            c.readfromstring(line);
            champs.push_back(c);
        }
    );
    EXPECT_TRUE(champexiststest(champs,"akali"));
    EXPECT_TRUE(champexiststest(champs,"alistar"));
    EXPECT_TRUE(champexiststest(champs,"braum"));
}