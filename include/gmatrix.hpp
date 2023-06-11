#ifndef AX_GMATRIX_HPP
#define AX_GMATRIX_HPP

#include <initializer_list>
#include <stdexcept>
#include <vector>
#include <functional>
#include <numeric>

#include "indexers.hpp"

namespace ax {

template <typename T>
class gmatrix {
private:
    std::vector<T> data;
    std::vector<std::size_t> shape;
    std::size_t idx_mult;

    gmatrix (std::vector<std::size_t>::iterator shape_begin, std::vector<std::size_t>::iterator shape_end) 
        : data(std::accumulate(shape_begin, shape_end, 1, std::multiplies<std::size_t>())), 
          shape(shape_begin, shape_end),
          idx_mult(std::accumulate(shape_begin + 1, shape_end, 1, std::multiplies<std::size_t>()))
        { }

    gmatrix (std::vector<std::size_t> shape) 
        : gmatrix(shape.begin(), shape.end()) { }

    gmatrix (
        std::vector<std::size_t>::iterator shape_begin, 
        std::vector<std::size_t>::iterator shape_end, 
        typename std::vector<T>::iterator data_begin,
        typename std::vector<T>::iterator data_end
    ) 
        : data(data_begin, data_end), 
          shape(shape_begin, shape_end),
          idx_mult(std::accumulate(shape_begin + 1, shape_end, 1, std::multiplies<std::size_t>()))
        { }

public:
    gmatrix (std::initializer_list<std::size_t> shape) 
        : gmatrix(std::vector<std::size_t>(shape)) { }

    // Slice the tensor, returns a new tensor
    gmatrix<T> operator[] (std::size_t idx) {
        if (idx >= shape[0])
            throw std::out_of_range("Index out of boundaries");
        std::vector<std::size_t> result_shape = 
            shape.size() == 1 ? 
            std::vector<std::size_t>({1}) : 
            std::vector<std::size_t>(shape.begin() + 1, shape.end());
        return gmatrix<T>(
            result_shape.begin(), 
            result_shape.end(), 
            data.begin() + idx * idx_mult, 
            data.end()
        );
    }

    gmatrix<T> operator[] (range r) {
        std::size_t start = r.start, end = r.end, step = r.step;
        if (start >= shape[0] || end > shape[0])
            throw std::out_of_range("Index out of boundaries");
        std::size_t n_slices = (end - start + 1) / step;
        
        // WIP
    }



    std::size_t size() const {
        return data.size();
    }

    std::vector<std::size_t> get_shape() const {
        return shape;
    }

    operator T&() const {
        if (shape.size() != 1 || shape[0] != 1)
            throw std::runtime_error("Cannot cast matrix to T.");
        return data[0];
    }

    /**
     * Returns a reference to the indexed element
    */
    T& get(std::initializer_list<std::size_t> indices) {
        return get(std::vector<std::size_t>(indices));
    }

    T& get(std::vector<std::size_t> indices) {
        if (indices.size() != shape.size()) 
            throw std::invalid_argument("Expected " + std::to_string(shape.size()) + " indices, got " + std::to_string(indices.size()));
        std::size_t idx = 0;
        std::size_t mult = 1;
        for (
            auto indices_it = indices.rbegin(), shape_it = shape.rbegin(); 
            indices_it != indices.rend(); indices_it++, shape_it++
        ) {
            if (*indices_it >= *shape_it)
                throw std::out_of_range("Index out of boundaries.");
            idx += mult * (*indices_it);
            mult *= shape_it != shape.rbegin() ? *(shape_it - 1) : 1;
        }
        return data[idx];
    }

    gmatrix<T> operator+(const gmatrix<T>& other) const {
        if (shape != other.shape)
            throw std::invalid_argument("Cannot add matrices of different shapes.");
        gmatrix<T> result(shape);
        for (std::size_t i = 0; i < size(); i++)
            result.data[i] = data[i] + other.data[i];
        return result;
    }

    gmatrix<T> operator-(const gmatrix<T>& other) const {
        if (shape != other.shape)
            throw std::invalid_argument("Cannot subtract matrices of different shapes.");
        gmatrix<T> result(shape);
        for (std::size_t i = 0; i < size(); i++)
            result.data[i] = data[i] - other.data[i];
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, gmatrix<T>& matrix) {
        os << "[";
        if (matrix.shape.size() == 1 && matrix.shape[0] == 1)
            os << matrix.data[0] << ", ";
        else if (matrix.shape.size() == 1) {
            for (std::size_t i = 0; i < matrix.shape[0]; i++)
                os << matrix.data[i] << ((i == (matrix.shape[0] - 1)) ? "" : ", ");
        }
        else
            for (std::size_t i = 0; i < matrix.shape[0]; i++) {
                gmatrix<T> slice = matrix[i];
                os << slice << ((i == (matrix.shape[0] - 1)) ? "" : ", ");
            }
        os << "]";
        return os;
    }
};

}

#endif