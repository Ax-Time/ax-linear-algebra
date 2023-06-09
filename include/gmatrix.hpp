#ifndef AX_MATRIX_HPP
#define AX_MATRIX_HPP

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <vector>
#include <iterator>
#include <iostream>

namespace ax {

template <typename T>
class indexer {
private:
    std::vector<size_t>* shape;
    T** data;
    std::vector<size_t> indices;
public:
    indexer(std::vector<size_t>* shape, T** data, size_t idx) {
        this->shape = shape;
        this->data = data;
        this->indices.push_back(idx);
    }

    indexer operator[] (size_t idx) {
        this->indices.push_back(idx);
        return *this;
    }

    operator T&() const {
        if (indices.size() != this->shape->size()) {
            std::string err;
            err += "Matrix requires ";
            err += std::to_string(this->shape->size());
            err += " indices, ";
            err += std::to_string(indices.size());
            err += " provided.";
            throw std::runtime_error(err);
        } 

        size_t idx = 0;
        for (size_t i = 0; i < indices.size(); i++) {
            size_t curr_idx = indices[i];
            size_t multiplier = 1;
            for (size_t j = i + 1; j < indices.size(); j++) {
                multiplier *= (*this->shape)[j];
            }
            idx += curr_idx * multiplier;
        }

        return (*this->data)[idx];
    } 

    void operator = (T value) {
        (T&) (*this) = value;
    }
};

/**
 * Generalized matrix implementation (can be used for vectors, matrices and tensors).
*/
template <typename T>
class gmatrix {
private:
    std::vector<size_t> shape;
    T* data;
    size_t size;
public:
    gmatrix(std::initializer_list<size_t> shape) {
        if (shape.size() == 0) throw std::invalid_argument("Cannot create matrix from empty shape");
        size_t size = 1;
        for (auto s : shape) {
            if (s == 0) throw std::invalid_argument("Cannot create zero-shaped matrix.");
            this->shape.push_back(s);
            size *= s;
        }
        data = new T[size];
        this->size = size;
    }

    indexer<T> operator[] (size_t idx) {
        return indexer<T>(&this->shape, &this->data, idx);
    }

    friend class indexer<T>;
};

}

#endif