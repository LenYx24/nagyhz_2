#ifndef LOLMACROGAME_IOPARSER_H
#define LOLMACROGAME_IOPARSER_H
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include "gameobjects.hpp"
namespace IOParser{
/**
 * @brief a file holder that closes the file
 */
  class File{
  public:
    /**
     * @brief opens the file at the given path, throws error if the path is wrong
     * @param path the filepath
     */
    explicit File(const std::filesystem::path& path);
    ~File(){file.close();}
    /**
     * @brief gets the opened file
     * @return the file
     */
    std::fstream &getfile(){return file;}
  private:
    std::fstream file;
  };
  std::vector<std::string> split_string(const std::string& str, char delimiter);
  Champion *create_champ(const std::string &line);
  Item create_item(const std::string &line);
}
#endif // LOLMACROGAME_IOPARSER_H
