#include <exception>
#include <iostream>

#include "Program.h"

int main(int argc, char **argv) {

    try {
        Program program(argv[0]);
        program.run();
    } catch (std::exception &e) {
        std::cerr << "CRITICAL: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
