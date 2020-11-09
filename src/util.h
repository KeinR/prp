#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <string>

namespace ut {
    char getChar(bool printChar = true);
    void clear();
    std::string dirname(const std::string &path);
    std::string toLowercase(const std::string &str);
}

#endif
