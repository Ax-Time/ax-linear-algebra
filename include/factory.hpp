#ifndef AX_FACTORY_HPP
#define AX_FACTORY_HPP

#include "gmatrix.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>

namespace ax {

class startup
{
    public:
        static startup& getInstance()
        {
            static startup instance; // Guaranteed to be destroyed.
                                     // Instantiated on first use.
            return instance;
        }
        void init() {
            std::srand(std::time(0));
        }
    private:
        startup() {}                    // Constructor? (the {} brackets) are needed here.
    public:
        startup(startup const&)         = delete;
        void operator=(startup const&)  = delete;

        // Note: Scott Meyers mentions in his Effective Modern
        //       C++ book, that deleted functions should generally
        //       be public as it results in better error messages
        //       due to the compilers behavior to check accessibility
        //       before deleted status
};

namespace factory {

gmatrix<float> randn(std::initializer_list<size_t> shape, float mean, float stddev) {
    startup::getInstance().init();
    gmatrix<float> m(shape);

    for (size_t i = 0; i < m.get_size(); i++) {
        m.get_indexer().flat(i) = mean + (std::rand() / (float) RAND_MAX) * stddev;
    }
    return m;
}

}

}

#endif
