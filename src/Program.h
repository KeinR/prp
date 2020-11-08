#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <vector>
#include <string>

class Program {
    std::vector<int> adv(std::vector<int> &v);
    std::vector<int> getExpansion(int count);
    void pause();
    void print(const std::string &str);
    void printLines(const std::string &str);
public:
    Program();
    void run();
};

#endif
