#ifndef CODEC_H_INCLUDED
#define CODEC_H_INCLUDED

#include <string>

namespace cd {
    bool testEscape(const std::string &in, std::string::size_type i);
    void passEscape(const std::string &in, std::string::size_type &i);

    bool testPause(const std::string &in, std::string::size_type i);
    std::string compilePause(const std::string &in, std::string::size_type &i);
    bool isPause(const std::string &in, std::string::size_type &i);

    bool testFunc(const std::string &in, std::string::size_type i);
    bool testCmpFunc(const std::string &in, std::string::size_type i);
    std::string compileFunc(const std::string &in, std::string::size_type &i);
    std::string readFunc(const std::string &in, std::string::size_type &i);

    bool testShortcode(const std::string &in, std::string::size_type i);
    std::string compileShortcode(const std::string &in, std::string::size_type &i);

    std::string mapDegree(int v);
}

#endif
