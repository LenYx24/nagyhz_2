#ifndef IOPARSER_HPP
#define IOPARSER_HPP
#include <fstream>
#include <iostream>

class Ireadstring {
public:
  virtual void readfromstring(std::string &line, const char delimiter = ';') = 0;
};
class iofile {
public:
  iofile(std::string path);
  ~iofile();
  std::ifstream &getfile() {
    return file;
  }

private:
  std::ifstream file;
};
#endif
