#ifndef AX_INDEXERS_HPP
#define AX_INDEXERS_HPP

#include <vector>

namespace ax {
    class range {
    public:
        const std::size_t start, end, step;
        
        range(std::size_t start, std::size_t end, std::size_t step = 1) : start(start), end(end), step(step) { }
        range(std::size_t end) : range(0, end) { }
    };
}

#endif
