#include "codec.h"

#include <iostream>
#include <cassert>

/*
* Shortcode syntax:
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
static constexpr char SC_START = '@';

static constexpr char FUNC_FLAG = '@';
static constexpr char FUNC_OPEN = '[';
static constexpr char FUNC_CLOSE = ']';
static constexpr char PAUSE = '\x1F';
static constexpr char PAUSE_CHR = '#';
static constexpr char ESCAPE = '\\';

static constexpr char FUNC_BC = '\x10';

static std::string mapShortcode(char g, char v);

bool cd::testEscape(const std::string &in, std::string::size_type i) {
    return in[i] == ESCAPE && i + 1 < in.size() &&
        (testFunc(in, i+1) || testPause(in, i+1) || testShortcode(in, i+1));
}
void cd::passEscape(const std::string &in, std::string::size_type &i) {
    i++;
}

bool cd::testPause(const std::string &in, std::string::size_type i) {
    return in[i] == PAUSE_CHR;
}
std::string cd::compilePause(const std::string &in, std::string::size_type &i) {
    i++;
    std::string result;
    result.push_back(PAUSE);
    return result;
}
bool cd::isPause(const std::string &in, std::string::size_type &i) {
    if (in[i] == PAUSE) {
        i++;
        return true;
    }
    return false;
}

bool cd::testFunc(const std::string &in, std::string::size_type i) {
    return in[i] == FUNC_FLAG &&
        i + 2 < in.size() &&
        in[i + 1] == FUNC_OPEN &&
        // No no-name calls
        in[i + 2] != FUNC_CLOSE;
}
bool cd::testCmpFunc(const std::string &in, std::string::size_type i) {
    return in[i] == FUNC_BC;
}
std::string cd::compileFunc(const std::string &in, std::string::size_type &i) {
    std::string::size_type mi = i + 2;
    std::string result;
    result.push_back(FUNC_BC);
    for (;; mi++) {
        if (mi >= in.size()) {
            return "";
        } else if (in[mi] == FUNC_CLOSE) {
            break;
        }
        result.push_back(in[mi]);
    }
    result.push_back(FUNC_BC);
    i = mi + 1;
    return result;
}
std::string cd::readFunc(const std::string &in, std::string::size_type &i) {
    // unlikely to fail
    std::string::size_type mi = i + 1;
    std::string result;
    for (;; mi++) {
        if (mi >= in.size()) {
            std::cerr << "WARNING: Failed to read compiled function call" << '\n';
            return "";
        } else if (in[mi] == FUNC_BC) {
            break;
        }
        result.push_back(in[mi]);
    }
    i = mi + 1;
    return result;
}

bool cd::testShortcode(const std::string &in, std::string::size_type i) {
    return in[i] == SC_START && i + 2 < in.size() && in[i+1] != FUNC_OPEN;
}
std::string cd::compileShortcode(const std::string &in, std::string::size_type &i) {
    std::string mapping = mapShortcode(in[i + 1], in[i + 2]);
    if (mapping.size() > 0) {
        // Was successful
        i += 3;
    }
    return mapping;
}

std::string cd::mapDegree(int v) {
    std::string result;
    if (v < 0) {
        result.append("¯");
    }

    do {
        int digit = v % 10;
        v /= 10;
        const char *s = nullptr;
        switch (digit) {
            case 0: s = "⁰"; break;
            case 1: s = "¹"; break;
            case 2: s = "²"; break;
            case 3: s = "³"; break;
            case 4: s = "⁴"; break;
            case 5: s = "⁵"; break;
            case 6: s = "⁶"; break;
            case 7: s = "⁷"; break;
            case 8: s = "⁸"; break;
            case 9: s = "⁹"; break;
            default: assert(false); break; // Should never happen
        }
        result.insert(0, s);
    } while (v != 0);

    return result;
}

std::string mapShortcode(char g, char v) {
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
