

/*  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MATRIX_HPP_
#define MATRIX_HPP_

#include <algorithm>
#include <stdexcept>

namespace gc {

template<class T>
class matrix {
public:
    typedef T               value_type;
    typedef T*              pointer;
    typedef const T*        const_pointer;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef T*              iterator;
    typedef const T*        const_iterator;
    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;
public:
    matrix();
    matrix(const matrix& o);
    matrix(size_type width, size_type height, const T& init_value = T());

    matrix& operator=(const matrix& o);

    virtual ~matrix();

    bool operator==(const matrix& o) const;
    bool operator!=(const matrix& o) const;

    reference at(size_type x, size_type y);
    const_reference at(size_type x, size_type y) const;

    reference operator()(size_type x, size_type y);
    const_reference operator()(size_type x, size_type y) const;

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;

    size_type width() const;
    size_type height() const;

    void swap(matrix<T>& rhs);
protected:
    size_type width_;
    size_type height_;
    T *data_;
};

template<class T>
matrix<T>::matrix() : width_(0), height_(0), data_(0) {}

template<class T>
matrix<T>::matrix(const matrix<T>& o) : width_(o.width_), height_(o.height_) {
    if ( o.data_ ) {
        data_ = new T[width_ * height_];
        std::copy( o.begin(), o.end(), begin() );
    } else {
        data_ = 0;
    }
}

template<class T>
matrix<T>::matrix(size_type width, size_type height, const T& init_value) :
    width_(width),
    height_(height) {
    if ( width_ == 0 || height_ == 0 ) {
        data_ = 0;
    } else {
        data_ = new T[width_*height_];
        std::fill(data_, data_ + (width_*height_), init_value);
    }
}

template<class T>
matrix<T>& matrix<T>::operator=(const matrix<T>& o) {
    if ( this == &o ) {
        return *this;
    }

    delete[] data_;

    width_ = o.width_;
    height_ = o.height_;
    if ( o.data_ ) {
        data_ = new T[width_ * height_];
        std::copy( o.begin(), o.end(), begin() );
    } else {
        data_ = 0;
    }
    return *this;
}

template<class T>
matrix<T>::~matrix() {
    delete[] data_;
}

template<class T> inline
bool matrix<T>::operator==(const matrix<T>& o) const {
    return width_ == o.width_ && height_ == o.height && std::equal( data_, data_ + width_*height_, o.data_);
}

template<class T> inline
bool matrix<T>::operator!=(const matrix<T>& o) const {
    return !(*this == o);
}

template<class T> inline
typename matrix<T>::reference matrix<T>::at(size_type x, size_type y) {
    if ( x >= width_ || y >= height_ )
        throw std::out_of_range("matrix : Out of range");
    return this->operator()(x,y);
}

template<class T> inline
typename matrix<T>::const_reference matrix<T>::at(size_type x, size_type y) const {
    if ( x >= width_ || y >= height_ )
        throw std::out_of_range("matrix : Out of range");
    return this->operator()(x,y);
}

template<class T> inline
typename matrix<T>::reference matrix<T>::operator()(size_type x, size_type y) {
    return data_[y*width_+x];
}

template<class T> inline
typename matrix<T>::const_reference matrix<T>::operator()(size_type x, size_type y) const {
    return data_[y*width_+x];
}

template<class T> inline
typename matrix<T>::iterator matrix<T>::begin() {
    return data_;
}

template<class T> inline
typename matrix<T>::iterator matrix<T>::end() {
    return data_ + (width_*height_);
}

template<class T> inline
typename matrix<T>::const_iterator matrix<T>::begin() const {
    return data_;
}

template<class T> inline
typename matrix<T>::const_iterator matrix<T>::end() const {
    return data_ + (width_*height_);
}

template<class T> inline
typename matrix<T>::size_type matrix<T>::width() const {
    return width_;
}

template<class T> inline
typename matrix<T>::size_type matrix<T>::height() const {
    return height_;
}

template<class T> inline
void matrix<T>::swap(matrix<T>& rhs) {
    std::swap(width_, rhs.width_);
    std::swap(height_, rhs.height_);
    std::swap(data_, rhs.data_);
}

} //namespace gc

/*
 * This is illegal* according to the standard, but this is the only solution
 * *: maybe not: http://stackoverflow.com/questions/11562/how-to-overload-stdswap
 */
namespace std {

template<class T>
void swap(gc::matrix<T>& a, gc::matrix<T>& b) {
    a.swap(b);
}

} //namespace std

#endif /* MATRIX_HPP_ */
