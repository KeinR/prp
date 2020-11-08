#include "Program.h"

#include <iostream>

#include "util.h"

Program::Program() {
}

void Program::pause() {
    ut::getChar(false);
}

void Program::print(const std::string &str) {
    std::cout << str;
    std::cout.flush();
}
void Program::printLines(const std::string &str) {
    for (char c : str) {
        if (c == '\n') {
            std::cout.flush();
            pause();
        }
        std::cout << c;
    }
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
        print(R"(
______________________________________________________________________________________________________________
/                                                                                                              \
|                                      POWER RULE PROOF                                                        |
|                         ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~ ~                                      |
|                                  \ebrought to you by MinTTY                                                    |
|                            Written and directed by Orion Musselman                                           |
|                          Special thanks to the Gensuko                                                       |
|                                                                                                              |
|                                                                                                              |

)");

    pause();

//     printLines("""
// ______________________________________________________________________________________________________________
// /                                                                                                              \
// |                                 Today, we are going to prove:
//                                   d/dx xⁿ      =      nxⁿ-¹
//                                   Using the limit definition of a derivative.
                                  
//                                   We'll be using the more common definition, that is:
                                  
//                                 d/dx f(x) =  lim  ( f(x + h) - f(x) ) / h
//                                                 h->0

//     """);

}
