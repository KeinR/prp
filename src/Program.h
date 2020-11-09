#ifndef PROGRAM_H_INCLUDED
#define PROGRAM_H_INCLUDED

#include <vector>
#include <string>
#include <map>
#include <functional>

class Program {
    typedef std::function<void()> func_t;
    typedef std::map<std::string, std::string> slides_t;
    typedef std::map<std::string, func_t> funcs_t;

    slides_t slides;
    funcs_t funcs;

    std::vector<int> adv(std::vector<int> &v);
    std::vector<int> getExpansion(int count);
    std::string getExpansionStr(int count);
    std::string &getSlide(const std::string &name);
    void pause();
    void print(const std::string &name);
    std::string process(const std::string &source);
    void callFunc(const std::string &name);
    void flush(std::string &buffer);
    void printExp(int d);
    void addFunc(const std::string &name, const func_t &f);
public:
    Program(const std::string &execPath);
    void run();
};

#endif
