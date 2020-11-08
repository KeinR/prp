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
                std::cout << "WARNING: Failed to open " << e.path() << '\n';
            }

            file.seekg(0, file.end);
            int length = file.tellg();
            file.seekg(0, file.beg);

            std::vector<char> buffer(length);
            file.read(buffer.data(), buffer.size());

            if (!file.good()) {
                std::cout << "WARNING: Failed while reading from " << e.path() << '\n';
            }
            file.close();

            slides[e.path().filename().string()] = process(std::string(buffer.cbegin(), buffer.cend()));
        }
    } catch (fs::filesystem_error &e) {
        throw std::runtime_error("Cannot load slides from directory [" + slidesDir + "]");
    }
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
    for (char c : s) {
        if (c == cd::PAUSE) {
            std::cout << buffer;
            buffer.clear();
            std::cout.flush();
            pause();
        } else {
            buffer.push_back(c);
        }
    }
    std::cout << buffer;
    std::cout.flush();
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

    print("prep");

    pause();

    print("beginwork");

    pause();

    print("continue");
}

/*
* syntax:
* [group][val]
* (nowin = not supported by windows terminal)
* 0 - Basic
*   0 - RESET ALL attributes
*   1 - bold
*   2 - faint (nowin)
*   3 - italics (nowin)
*   4 - underline
*   5 - double underline (nowin)
*   6 - strikethrough (nowin)
*   7 - RESET color and intensity (bold/faint)
*   8 - DISABLE italics
*   9 - DISABLE underline
*   a - DISABLE crossout
* [
* 1 - Foreground
* 2 - Bright foreground
* 3 - Background
* 4 - Bright background
* ]
*   0 - Black
*   1 - Red
*   2 - Green
*   3 - Yellow
*   4 - Blue
*   5 - Magenta
*   6 - Cyan
*   7 - White
*
* Returns empty string on fail
*/

std::string Program::process(const std::string &source) {
    typedef std::string::size_type size;

    std::string result;
    result.reserve(source.size() * 1.2);

    for (size i = 0; i < source.size();) {
        bool skip = false;
        switch (source[i]) {
            case cd::SC_START:
                if (i + 2 < source.size()) {
                    std::string mapping = mapShortcode(source[i + 1], source[i + 2]);
                    if (mapping.size() > 0) {
                        i += 3;
                        result += mapping;
                        skip = true;
                        continue;
                    }
                }
                break;
            case cd::PAUSE_CHR:
                result.push_back(cd::PAUSE);
                i++;
                skip = true;
                break;
            case cd::ESCAPE:
                if (i + 1 < source.size()) {
                    switch (source[i + 1]) {
                        case cd::SC_START: // Fallthrough
                        case cd::PAUSE_CHR: // Fallthrough
                        case cd::ESCAPE:
                            i++;
                            // Will go straight to append
                            break;
                        // How about NO
                        // case '\r': // Fallthrough
                        //     if (i + 2 >= source.size() || source[i + 2] != '\n') {
                        //         break;
                        //     }
                        //     i++;
                        // case '\n':
                        //     i += 2;
                        //     break;
                    }
                }
                break;
            case '\r': // God forsaken carriage returns, go to ****!
                i++;
                skip = true;
                break;
        }

        if (!skip && i < source.size()) {
            result.push_back(source[i]);
            i++;
        }
    }

    result.shrink_to_fit();

    return result;
}

std::string Program::mapShortcode(char g, char v) {
    std::string code;
    switch (g) {
        case '0':
            switch (v) {
                case '0':
                    code = "0";
                    break;
                case '1':
                    code = "1";
                    break;
                case '2':
                    code = "2";
                    break;
                case '3':
                    code = "3";
                    break;
                case '4':
                    code = "4";
                    break;
                case '5':
                    code = "21";
                    break;
                case '6':
                    code = "9";
                    break;
                case '7':
                    code = "22";
                    break;
                case '8':
                    code = "23";
                    break;
                case '9':
                    code = "24";
                    break;
                case 'a':
                    code = "25";
                    break;
            }
            break;
        default:
            if (v >= '0' && v <= '7') {
                switch (g) {
                    case '1':
                        code = "30";
                        break;
                    case '2':
                        code = "90";
                        break;
                    case '3':
                        code = "40";
                        break;
                    case '4':
                        code = "100";
                        break;
                }
                if (code.size()) {
                    code.back() = v;
                }
            }
            break;
    }
    std::string result;
    if (code.size() > 0) {
        result += "\e[";
        result += code;
        result += "m";
        return result;
    }
    return result;
}
