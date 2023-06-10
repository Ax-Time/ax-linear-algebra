#ifndef AX_FMATRIX_HPP
#define AX_FMATRIX_HPP

#include "gmatrix.hpp"

#include <vector>
#include <stdexcept>

namespace ax {

class fmatrix : public gmatrix<float> {
public:
    fmatrix (size_t m, size_t n) 
        : gmatrix(m, n) {}
};

}

#endif