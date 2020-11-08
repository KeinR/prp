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

    
    // try {
    //     int count = std::stoi(argv[1]);
    //     std::cout << "(a + b)^" << count << "\n";
    //     std::vector<int> qt = getExpansion(count);
    //     for (int i = 0; i <= count; i++) {
    //         if (qt[i] != 1) {
    //             std::cout << qt[i];
    //         }
    //         if (i != count) {
    //             std::cout << 'x';
    //             if (i != count - 1) {
    //                 std::cout << '^' << (count - i);
    //             }
    //         }
    //         if (i != 0) {
    //             std::cout << 'b';
    //             if (i != 1) {
    //                 std::cout << '^' << i;
    //             }
    //         }
    //         if (i != count) {
    //             std::cout << " + ";
    //         }
    //     }
    //     std::cout.flush();
    // } catch (std::exception &e) {
    //     std::cerr << argv[1] << " must be an integer" << '\n';
    //     return 1;
    // }
}




