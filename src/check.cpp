#include <iostream>

int main() {
#if __cplusplus == 199711L
    std::cout << "C++98/03\n";
#elif __cplusplus == 201103L
    std::cout << "C++11\n";
#elif __cplusplus == 201402L
    std::cout << "C++14\n";
#elif __cplusplus == 201703L
    std::cout << "C++17\n";
#elif __cplusplus == 202002L
    std::cout << "C++20\n";
#elif __cplusplus > 202002L
    std::cout << "C++23 or newer\n";
#else
    std::cout << "Unknown C++ version: " << __cplusplus << "\n";
#endif
}

