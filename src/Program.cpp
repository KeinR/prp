#include "Program.h"

#include <iostream>
#include <stdexcept>
#include <filesystem>
#include <sstream>
#include <fstream>

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

            slides[e.path().filename().string()] = process(std::string(buffer.cbegin(), buffer.cend()));
        }
    } catch (fs::filesystem_error &e) {
        throw std::runtime_error("Cannot load slides from directory [" + slidesDir + "]");
    }

    funcs["expand"] = []()->void{
        std::cout << "[expansion successful]";
    };

    funcs["fortune"] = []()->void{
        system("fortune");
    };
}

void Program::pause() {
    ut::getChar(false);
}

std::string &Program::getSlide(const std::string &name) {
    slides_t::iterator it = slides.find(name);
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

void Program::run() {
    print("begin");

    pause();

    print("overview");

    pause();

    print("BuTwHYtHo");

    pause();

    print("prep");

    pause();

    print("beginwork");

    pause();

    print("solution");

    pause();

    print("continue");

    pause();

    print("review");
}

std::string Program::process(const std::string &source) {
    typedef std::string::size_type size;

    std::string result;
    result.reserve(source.size() * 1.2);

    for (size i = 0; i < source.size();) {

        if (cd::testEscape(source, i)) {
            cd::passEscape(source, i);
            result.push_back(source[i]);
        } else if (cd::testPause(source, i)) {
            result += cd::compilePause(source, i);
        } else if (cd::testFunc(source, i)) {
            result += cd::compileFunc(source, i);
        } else if (cd::testShortcode(source, i)) {
            result += cd::compileShortcode(source, i);
        } else {
            result.push_back(source[i]);
            i++;
        }
    }

    result.shrink_to_fit();

    return result;
}

void Program::callFunc(const std::string &name) {
    funcs_t::iterator it = funcs.find(name);
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
