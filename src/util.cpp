#include "util.h"

#include "platform.h"

#include <iostream>

#ifdef IS_OS_WINDOWS
#   include <conio.h>
#else
#   include <unistd.h>
#   include <termios.h>
#endif

char ut::getChar(bool printChar) {
    #ifdef IS_OS_WINDOWS

    return _getch();

    #else

    // ty stack https://stackoverflow.com/a/912796/10821333
    char buf = 0;
    struct termios old = {0};
    fflush(stdout);
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    if (printChar) {
        printf("%c\n", buf);
    }
    return buf;

    #endif // IS_OS_WINDOWS
}

void ut::clear() {
    #ifdef IS_OS_WINDOWS

    system("cls");

    #else

    system("clear");

    #endif
}

std::string ut::dirname(const std::string &path) {
    std::string result = path;
    for (std::string::size_type i = result.size()-1; i < result.size(); i--) {
        if (result[i] == '/') {
            if (i+1 < result.size()) {
                result.erase(i+1);
            }
            break;
        }
    }
    return result;
}
