#include <iostream>
#include <string_view>

namespace __internal {
    extern int getgid();
    extern int getpid();
    extern int getuid();
}

int main(int argc, char* argv[]) {
    std::string_view svHello = "Hello! (";
    //std::string_view svSp = " ";
    std::string_view svN = ")\n";

    std::cout << svHello << __internal::getpid() << svN;

    return 0;
}
