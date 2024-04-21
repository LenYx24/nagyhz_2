#include "../include/ioparser.hpp"

iofile::iofile(std::string path) : file(path) {}
iofile::~iofile() {
  file.close();
}
