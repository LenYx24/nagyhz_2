#ifndef LOLMACROGAME_IOPARSER_H
#define LOLMACROGAME_IOPARSER_H
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "gameobjects.hpp"
namespace IOParser{
  class File{
  public:
    File(const std::filesystem::path& path);
    ~File(){file.close();}
    std::fstream &getfile(){return file;}
  private:
    std::fstream file;
  };
  std::vector<std::string> split_string(const std::string& str, char delimiter);
  Champion *create_champ(const std::string &line);
  Item create_item(const std::string &line);
}
#endif // LOLMACROGAME_IOPARSER_H
