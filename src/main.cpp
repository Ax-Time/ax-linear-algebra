#include <iostream>
#include <string>

#include <cstdlib>
#include <ctime>

#include "gmatrix.hpp"
#include "factory.hpp"

int main() {
    ax::gmatrix<float> M({2, 2}), N({2, 2});

    M.get({0, 0}) = 1;
    N.get({0, 0}) = 2;

    auto C = M + N;

    std::cout << C << std::endl;

    return 0;
}