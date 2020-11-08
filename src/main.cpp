#include <iostream>
#include <string>
#include <exception>

#include <vector>

std::vector<int> adv(std::vector<int> &v) {
    std::vector<int> result;
    result.reserve(v.size() + 2);
    for (unsigned int i = 0; i <= v.size(); i++) {
        int l = i - 1 < v.size() ? v[i - 1] : 0;
        int n = i < v.size() ? v[i] : 0;
        result.push_back(l + n);
    }
    return result;
}

std::vector<int> getExpansion(int count) {
    std::vector<int> result;
    result.push_back(1);
    for (int i = 0; i < count; i++) {
        result = adv(result);
    }
    return result;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [integer]" << '\n';
        return 1;
    }
    
    try {
        int count = std::stoi(argv[1]);
        std::cout << "(a + b)^" << count << "\n";
        std::vector<int> qt = getExpansion(count);
        for (int i = 0; i <= count; i++) {
            if (qt[i] != 1) {
                std::cout << qt[i];
            }
            if (i != count) {
                std::cout << 'x';
                if (i != count - 1) {
                    std::cout << '^' << (count - i);
                }
            }
            if (i != 0) {
                std::cout << 'b';
                if (i != 1) {
                    std::cout << '^' << i;
                }
            }
            if (i != count) {
                std::cout << " + ";
            }
        }
        std::cout.flush();
    } catch (std::exception &e) {
        std::cerr << argv[1] << " must be an integer" << '\n';
        return 1;
    }
}




