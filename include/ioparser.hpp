#ifndef IOPARSER_HPP
#define IOPARSER_HPP
#include <fstream>
#include <iostream>

class Ireadstring {
public:
  virtual ~Ireadstring();
  virtual void read_from_string(std::string &line, const char delimiter = ';') = 0;
};
// this interface should be inherited by classes that need to put out their data/results
class Iwritestring {
  virtual std::string writetostring(const char delimiter = ';') = 0;
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
