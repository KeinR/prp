#include "Program.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <array>

#include "util.h"
#include "codec.h"

namespace fs = std::filesystem;

Program::Program(const std::string &execPath) {
    std::string dir = ut::dirname(execPath);

    std::string slidesDir = dir + "slides";

    try {
        for (fs::directory_entry e : fs::directory_iterator(slidesDir)) {
            std::ifstream file(e.path());
            if (!file.good()) {
                std::cerr << "WARNING: Failed to open " << e.path() << '\n';
            }

            file.seekg(0, file.end);
            int length = file.tellg();
            file.seekg(0, file.beg);

            std::vector<char> buffer(length);
            file.read(buffer.data(), buffer.size());

            if (!file.good()) {
                std::cerr << "WARNING: Failed while reading from " << e.path() << '\n';
            }
            file.close();

            slides[ut::toLowercase(e.path().filename().string())] = process(std::string(buffer.cbegin(), buffer.cend()));
        }
    } catch (fs::filesystem_error &e) {
        throw std::runtime_error("Cannot load slides from directory [" + slidesDir + "]");
    }

    addFunc("expand", [this]()->void{
        printExp(8);
        printExp(20);
        printExp(50);
        printExp(100);
    });

    addFunc("expandMore", [this]()->void{
        printExp(500);
    });

    addFunc("fortune", [this]()->void{
        std::cout << process(R"(
Would you like to have a fortune (from fortune.exe) to celebrate your victory?
@03@02(@11DISCLAIMER@00@03@02: The author[s] are not responsible for the output of fortune.exe.
While the quotes are generally benign, I'm not the author of fortune.exe, so I have no idea what might
be printed out... some that I've seen could be considered controversial.
I'm just being cautious...)@00
)") << '\n';
        bool loop = true;
        bool consent = false;
        while (loop) {
            std::cout << "[y/n]";
            char c = ut::getChar(true);
            switch (c) {
                case 'y':
                case 'Y':
                    consent = true;
                    loop = false;
                    break;
                case 'n':
                case 'N':
                    loop = false;
                    break;
            }
        }
        if (consent) {
            std::cout << "Here you go:\n\n\e[1m" << std::flush;
            system("fortune");
            std::cout << "\e[0m" << '\n';
        }
    });
}

void Program::pause() {
    ut::getChar(false);
}

std::string &Program::getSlide(const std::string &name) {
    slides_t::iterator it = slides.find(ut::toLowercase(name));
    if (it != slides.end()) {
        return it->second;
    } else {
        throw std::invalid_argument("Cannot find slide " + name);
    }
}

void Program::print(const std::string &name) {
    ut::clear();
    std::string &s = getSlide(name);
    std::string buffer;
    buffer.reserve(s.size());
    for (std::string::size_type i = 0; i < s.size();) {
        if (cd::isPause(s, i)) {
            flush(buffer);
            pause();
        } else if (cd::testCmpFunc(s, i)) {
            flush(buffer);
            std::string name = cd::readFunc(s, i);
            callFunc(name);
        } else {
            buffer.push_back(s[i]);
            i++;
        }
    }
    flush(buffer);
}

std::vector<int> Program::adv(std::vector<int> &v) {
    std::vector<int> result;
    result.reserve(v.size() + 2);
    for (unsigned int i = 0; i <= v.size(); i++) {
        int l = i - 1 < v.size() ? v[i - 1] : 0;
        int n = i < v.size() ? v[i] : 0;
        result.push_back(l + n);
    }
    return result;
}

std::vector<int> Program::getExpansion(int count) {
    std::vector<int> result;
    result.push_back(1);
    for (int i = 0; i < count; i++) {
        result = adv(result);
    }
    return result;
}

std::string Program::getExpansionStr(int count) {
    std::stringstream result;
    std::vector<int> qt = getExpansion(count);
    for (int i = 0; i <= count; i++) {
        if (qt[i] != 1) {
            result << qt[i];
        }
        if (i != count) {
            result << 'x';
            if (i != count - 1) {
                result << cd::mapDegree(count - i);
            }
        }
        if (i != 0) {
            result << 'b';
            if (i != 1) {
                result << cd::mapDegree(i);
            }
        }
        if (i != count) {
            result << " + ";
        }
    }
    result.flush();
    return result.str();
}

void Program::run() {

    std::array<std::string, 9> slides = {
        "begin",
        "overview",
        "BuTwHYtHo",
        "prep",
        "beginwork",
        "solution",
        "continue",
        "review",
        "credits"
    };

    for (std::string s : slides) {
        print(s);
        pause();
    }
}

std::string Program::process(const std::string &source) {
    typedef std::string::size_type size;

    std::string result;
    result.reserve(source.size() * 1.2);

    for (size i = 0; i < source.size();) {

        if (cd::testEscape(source, i)) {
            cd::passEscape(source, i);
            result.push_back(source[i]);
            i++;
        } else if (cd::testPause(source, i)) {
            result += cd::compilePause(source, i);
        } else if (cd::testFunc(source, i)) {
            result += cd::compileFunc(source, i);
        } else if (cd::testShortcode(source, i)) {
            std::string r = cd::compileShortcode(source, i);
            if (r.size() == 0) {
                result.push_back(source[i]);
                i++;
            } else {
                result += r;
            }
        } else {
            result.push_back(source[i]);
            i++;
        }
    }

    result.shrink_to_fit();

    return result;
}

void Program::callFunc(const std::string &name) {
    funcs_t::iterator it = funcs.find(ut::toLowercase(name));
    if (it != funcs.end()) {
        it->second();
    } else {
        std::cerr << "WARNING: No such native function named \"" << name << "\"" << '\n';
    }
}

void Program::flush(std::string &buffer) {
    std::cout << buffer;
    buffer.clear();
    std::cout.flush();
}

void Program::printExp(int d) {
    std::cout << process("@25(x + h)@24" + cd::mapDegree(d) + "@00") << " = " << getExpansionStr(d) << ",\n";
}

void Program::addFunc(const std::string &name, const func_t &f) {
    funcs[ut::toLowercase(name)] = f;
}
