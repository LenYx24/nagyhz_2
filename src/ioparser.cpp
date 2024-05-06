#include "../include/ioparser.hpp"

iofile::iofile(std::string path) : file(path) {}
Ireadstring::~Ireadstring(){}
iofile::~iofile() {
  file.close();
}
