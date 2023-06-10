#ifndef AX_GMATRIX_HPP
#define AX_GMATRIX_HPP

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
    indexer(std::vector<size_t>* shape, T** data) {
        this->shape = shape;
        this->data = data;
    }
    indexer(std::vector<size_t>* shape, T** data, size_t idx) {
        if (idx >= (*shape)[0]) {
            throw std::out_of_range("Index out of boundaries.");
        }
        this->shape = shape;
        this->data = data;
        this->indices.push_back(idx);
    }

    indexer operator[] (size_t idx) {
        if (indices.size() == shape->size()) {
            throw std::runtime_error("Trying to index a scalar.");
        }
        if (idx >= (*shape)[indices.size()]) {
            throw std::out_of_range("Index out of boundaries.");
        }
        this->indices.push_back(idx);
        return *this;
    }

    void clear() {
        this->indices.clear();
    }

    void erase_last() {
        this->indices.pop_back();
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
    std::vector<size_t> _shape;
    size_t size;
public:
    T* data;
    gmatrix(std::initializer_list<size_t> shape) 
        : gmatrix(std::vector<size_t>(shape)) { }

    gmatrix(std::vector<size_t> shape) {
        if (shape.size() == 0) throw std::invalid_argument("Cannot create matrix from empty shape");
        size_t size = 1;
        for (auto s : shape) {
            if (s == 0) throw std::invalid_argument("Cannot create zero-shaped matrix.");
            this->_shape.push_back(s);
            size *= s;
        }
        data = new T[size];
        this->size = size;
    }

    gmatrix(size_t m, size_t n) 
        : gmatrix(std::vector<size_t>({m, n})) {}

    ~gmatrix() {
        delete[] data;
    }

    std::vector<size_t> shape() const {
        return this->_shape;
    }

    size_t get_size() const {
        return this->size;
    }
    
    /**
     * Matrix should be immutable, so we don't allow reshaping in place for now.
    */
    // void reshape(std::initializer_list<size_t> shape) {
    //     if (shape.size() == 0) throw std::invalid_argument("Cannot reshape matrix to empty shape");
    //     size_t after_size = 1;
    //     for (auto sh : shape) {
    //         if (sh == 0) throw std::invalid_argument("Cannot reshape matrix to zero shape.");
    //         after_size *= sh;
    //     }
    //     if (after_size != this->size) throw std::invalid_argument("Cannot reshape matrix to different size.");
    //     this->_shape.clear();
    //     for (auto sh : shape) {
    //         this->_shape.push_back(sh);
    //     }
    // }

    /**
     * Sums two tensors of the same shape
    */
    gmatrix operator+ (gmatrix<T>& other) {
        if (!same_shape(*this, other)) throw std::invalid_argument("Cannot add matrices with different shapes.");
        gmatrix<T> result(this->_shape);
        for (size_t i = 0; i < this->size; i++) {
            result.data[i] = this->data[i] + other.data[i];
        }
        return result;
    }

    /**
     * Computes the tensor product of two tensors:
     * A shape = (a1, a2, ..., an)
     * B shape = (b1, b2, ..., bm)
     * It can be computed only if an == b1
     * A x B shape = (a1, a2, ..., a(n-1), b2, ..., bm)
    */
    gmatrix operator* (gmatrix<T>& other) {
        if (!can_perform_product(*this, other)) throw std::invalid_argument("Cannot multiply uncompatible matrices.");
        std::vector<size_t> res_shape;
        res_shape.insert(res_shape.end(), _shape.begin(), _shape.end() - 1);
        res_shape.insert(res_shape.end(), other._shape.begin() + 1, other._shape.end());
        gmatrix<T> result(res_shape);
        memset(result.data, 0, result.size);
        std::vector<size_t> multipliers;
        multipliers.push_back(1);
        for (int i = result._shape.size() - 2; i >= 0; i--) {
            multipliers.insert(multipliers.begin(), *multipliers.begin() * result._shape[i + 1]);
        }

        // Start operation
        for (size_t idx = 0; idx < result.size; idx++) {
            std::vector<size_t> indices;

            size_t temp = idx;
            for (size_t mul : multipliers) {
                indices.push_back(temp / mul);
                temp = temp % mul;
            }
            
            indexer<T> result_indexer(&result._shape, &result.data);
            indexer<T> left_indexer(&this->_shape, &this->data);

            
            // Index the result matrix
            for (auto i : indices) {
                result_indexer = result_indexer[i];
            }

            // Index the left matrix
            for (size_t i = 0; i < this->_shape.size() - 1; i++) {
                left_indexer = left_indexer[indices[i]];
            }

            indexer<T> right_indexer(&other._shape, &other.data);
            for (size_t k = 0; k < other._shape[0]; k++) {
                auto r_indexer = right_indexer[k];

                // Index the right matrix
                for (size_t i = result._shape.size() - other._shape.size() + 1; i < indices.size(); i++) {
                    r_indexer = r_indexer[indices[i]];
                }

                T& value = result_indexer;
                T& left = left_indexer[k];
                T& right = r_indexer;
                value += left * right;

                // Clean up
                right_indexer.clear();
                left_indexer.erase_last();
            }
        }
        return result;
    }


    // Static methods
    static bool can_perform_product(gmatrix<T>& m1, gmatrix<T>& m2) {
        return m1._shape[m1._shape.size() - 1] == m2._shape[0];
    }

    static bool same_shape(gmatrix<T>& m1, gmatrix<T>& m2) {
        if (m1.shape().size() != m2.shape().size()) return false;
        for (size_t i = 0; i < m1.shape().size(); i++) {
            if (m1.shape()[i] != m2.shape()[i]) return false;
        }
        return true;
    }

    // Indexer
    indexer<T> operator[] (size_t idx) {
        return indexer<T>(&this->_shape, &this->data, idx);
    }

    friend class indexer<T>;
};

}

#endif