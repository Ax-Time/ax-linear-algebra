#include <iostream>
#include <string>

#include <cstdlib>
#include <ctime>

#include "gmatrix.hpp"
#include "fmatrix.hpp"
#include "factory.hpp"

int main() {
    auto m = ax::factory::randn({1, 1, 1}, 0, 1);

    std::cout << m << std::endl;

    return 0;
}