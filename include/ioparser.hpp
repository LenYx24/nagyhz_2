#ifndef IOPARSER_HPP
#define IOPARSER_HPP
#include <iostream>

class Iifile {
public:
    void readfromfile(const std::string &filepath, const char delimiter = ';');
};
#endif
