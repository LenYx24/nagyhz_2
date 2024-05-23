#include "../include/ioparser.h"
namespace IOParser{
  File::File(const std::filesystem::path& path){
    file.open(path);
    if(!file)
      throw std::invalid_argument("wrong file path");
  }
  std::vector<std::string> split_string(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
      tokens.push_back(token);
    }

    return tokens;
  }
  Champion *create_champ(const std::string &line){
    std::vector<std::string> tokens = IOParser::split_string(line, ';');
    if(tokens.size() < 5){
      throw std::runtime_error("wrong file format");
    }
    std::string name = tokens[0];
    double hp = std::stod(tokens[1]);
    double hp_per_level = std::stod(tokens[2]);
    double damage = std::stod(tokens[3]);
    double dmg_per_level = std::stod(tokens[4]);
    return new Champion{name,damage,dmg_per_level,hp,hp_per_level};
  }
  Item create_item(const std::string &line){
    std::vector<std::string> tokens = IOParser::split_string(line, ';');
    if(tokens.size() < 4){
      throw std::runtime_error("wrong file format");
    }
    std::string name = tokens[0];
    int gold_value = std::stoi(tokens[1]);
    double bonus_dmg = std::stod(tokens[2]);
    double bonus_hp = std::stod(tokens[3]);
    Item item{name,gold_value,bonus_dmg,bonus_hp};
    return item;
  }
}

