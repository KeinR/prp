#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <vector>
#include <string>
#include <map>

class Program {
    typedef std::map<std::string, std::string> slides_t;

    slides_t slides;

    std::vector<int> adv(std::vector<int> &v);
    std::vector<int> getExpansion(int count);
    std::string &getSlide(const std::string &name);
    void pause();
    void print(const std::string &name);
    std::string process(const std::string &source);
    std::string mapShortcode(char g, char v);
public:
    Program(const std::string &execPath);
    void run();
};

#endif
