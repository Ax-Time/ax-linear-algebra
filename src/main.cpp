#include <iostream>

#include <cstdlib>
#include <ctime>

#include "gmatrix.hpp"

int main() {
    ax::gmatrix<float> m1({6, 3, 4});
    ax::gmatrix<float> m2({4, 6, 3});

    // Initialize the matrices with subsequent numbers
    size_t counter = 0;
    for (size_t i = 0; i < m1.shape()[0]; i++) {
        for (size_t j = 0; j < m1.shape()[1]; j++) {
            for (size_t k = 0; k < m1.shape()[2]; k++) {
                m1[i][j][k] = counter++;
            }
        }
    }

    counter = 0;
    for (size_t i = 0; i < m2.shape()[0]; i++) {
        for (size_t j = 0; j < m2.shape()[1]; j++) {
            for (size_t k = 0; k < m2.shape()[2]; k++) {
                m2[i][j][k] = counter++;
            }
        }
    }

    // Product
    ax::gmatrix<float> m3 = m1 * m2;


    std::cout << m3[5][2][2][1] << std::endl;

    return 0;
}