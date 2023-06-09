#include <iostream>

#include "gmatrix.hpp"

int main() {
    ax::gmatrix<float> m({5, 2});

    std::cout << "Matrix shape: ";
    for (size_t i = 0; i < m.shape().size(); i++) {
        std::cout << m.shape()[i] << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < m.shape()[0]; i++) {
        for (size_t j = 0; j < m.shape()[1]; j++) {
            m[i][j] = i * j;
        }
    }

    for (size_t i = 0; i < m.shape()[0]; i++) {
        for (size_t j = 0; j < m.shape()[1]; j++) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    m.reshape({10});

    std::cout << "Matrix shape: ";
    for (size_t i = 0; i < m.shape().size(); i++) {
        std::cout << m.shape()[i] << " ";
    }
    std::cout << std::endl;

    for (size_t i = 0; i < m.shape()[0]; i++) {
        std::cout << m[i] << " ";
    }
    std::cout << std::endl;


    return 0;
}