#include "../include/ioparser.hpp"

iofile::iofile(std::string path) : file(path) {}
iofile::~iofile() {
  file.close();
}

void iofile::readfromfile(Ireadstring *readto, const char delimiter) {}
