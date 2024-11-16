#include <iostream>
#include <string_view>

namespace __internal {
    extern int getgid();
    extern int getpid();
    extern int getuid();
}

int main(int argc, char* argv[]) {
    std::string_view svHello = "Hello!\n";

    std::cout << "========== I ==========\n";
    std::cout << "PID: " << __internal::getpid() << "\n";
    std::cout << "UID: " << __internal::getuid() << "\n";
    std::cout << "GID: " << __internal::getgid() << "\n";
    std::cout << svHello;
    std::cout << "=======================\n";

    return 0;
}
