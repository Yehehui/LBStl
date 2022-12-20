#ifndef __HH_STL_ALGOBASE_H_
#define __HH_STL_ALGOBASE_H_

#include "hh_stl_pair.h"
#include "hh_type_traits.h"
#include "hh_stl_iterator.h"
#include "hh_stl_numeric.h"
#include<cstddef>
#include<cstring>
#include<iostream>

namespace hh_std {

template <class T>
struct equal_to: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        return x == y;
    }
};

template <class T>
struct no_equal_to: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        return x != y;
    }
};

template <class T>
struct greater: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        if(y < x)
            return true;
        return false;
    }
};

template <class T>
struct greater_equal: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        if(x < y) {
            return false;
        }
        return true;
    }
};

template <class T>
struct less: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        if(x < y)
            return true;
        return false;
    }
};

template <class T>
struct less_equal: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        if(y < x)
            return false;
        return true;
    }
};

template <class T>
struct logical_and: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        return x && y;
    }
};

template <class T>
struct logical_or: public hh_std::binary_function<T, T, bool> {
    bool operator() (const T &x, const T &y) const {
        return x || y;
    }
};

template <class T>
struct logical_not: public hh_std::unary_function<T, bool> {
    bool operator() (const T &x) const {
        return !x;
    }
};

template <class Iter1, class Iter2>
hh_std::pair<Iter1, Iter2> mismatch(Iter1 first, Iter1 last, Iter2 begin) {
    while(first != last) {
        if(*first != *begin) {
            return hh_std::make_pair(first, begin);
        }
        ++first;
        ++begin;
    }
    return hh_std::make_pair(last, begin);
}

template <class Iter1, class Iter2>
bool equal(Iter1 first, Iter1 last, Iter2 begin) {
    while(first != last) {
        if(*first != *begin)
            return false;
        ++first;
        ++begin;
    }
    return true;
}

template <class Iter1, class Iter2, class BinaryOps>
bool equal(Iter1 first, Iter1 last, Iter2 begin, BinaryOps op) {
    while(first != last) {
        if(!op(*first, *begin))
            return false;
        ++first;
        ++begin;
    }
    return true;
}

template <class InputIterator, class T>
void fill(InputIterator first, InputIterator last, const T &val) {
    while(first != last) {
        *first = val;
        ++first;
    }
}

template <class InputIterator, class Interge, class T>
void fill_n(InputIterator begin, Interge n, const T &val) {
    while(n) {
        *begin = val;
        ++begin;
        --n;
    }
}

template <class T>
T max(const T &x, const T &y) {
    if(x < y)
        return y;
    return x;
}

template <class T, class BinaryOps>
T max(const T &x, const T &y, BinaryOps &op) {
    if(op(x, y))
        return y;
    return x;
}

template <class T>
T min(const T &x, const T &y) {
    if(x < y)
        return x;
    return y;
}

template <class T, class BinaryOps>
T min(const T &x, const T &y, BinaryOps op) {
    if(op(x, y)) {
        return x;
    }
    return y;
}

template <class T>
void swap(T &x, T &y) {
    T tmp = x;
    x = y;
    y = tmp;
}

template <class InputIterator>
void iter_swap(InputIterator &x, InputIterator &y) {
    swap(*x, *y);
}

template <class Iter1, class Iter2>
bool lexicographical_compare(Iter1 first, Iter1 last, Iter2 first_x, Iter2 last_x) {
    while(first != last && first_x != last_x) {
        if(*first < *first_x) {
            return true;
        }
        if(*first > *first_x) {
            return false;
        }
        ++first;
        ++first_x;
    }
    return first_x != last_x;
}

template <class Iter1, class Iter2, class BinaryOps>
bool lexicographical_compare(Iter1 first, Iter1 last, Iter2 first_x, Iter2 last_x, BinaryOps op) {
    while(first != last && first_x != last_x) {
        if(op(*first, *first_x)) {
            return true;
        }
        if(op(*first_x, *first)) {
            return false;
        }
        ++first;
        ++first_x;
    }
    return first_x != last_x;
}

template <class Iter1, class Iter2>
Iter2 __copy_d(Iter1 first, Iter1 last, Iter2 dest) {
    size_t len = last - first;
    for(size_t pos = 0; pos < len; ++pos) {
        dest[pos] = first[pos];
    }
    return dest + len;
}

template <class Iter1, class Iter2>
Iter2 __copy(Iter1 first, Iter1 last, Iter2 dest, hh_std::random_access_iterator_tag, hh_std::random_access_iterator_tag) {
    return __copy_d(first, last, dest);
}

template <class Iter1, class Iter2>
Iter2 __copy(Iter1 first, Iter1 last, Iter2 dest, hh_std::input_iterator_tag, hh_std::input_iterator_tag) {
    while(first != last) {
        *dest = *first;
        ++first;
        ++dest;
    }
    return dest;
}

template <class Iter1, class Iter2>
Iter2 __copy(Iter1 first, Iter1 last, Iter2 dest, hh_std::input_iterator_tag, hh_std::output_iterator_tag) {
    while(first != last) {
        *dest = *first;
        ++first;
        ++dest;
    }
    return dest;
}

template <class T>
T *__copy_t(const T *first, const T *last, T *dest, __true_type) {
    memmove(dest, first, sizeof(T) * (last - first));
    return dest + (last - first);
}

template <class T>
T *__copy_t(const T *first, const T *last, T *dest, __false_type) {
    return __copy_d(first, last, dest);
}

template <class Iter1, class Iter2>
Iter2 __copy_dispatch(Iter1 first, Iter1 last, Iter2 dest) {
    typedef typename Iter1::iterator_category type1;
    typedef typename Iter2::iterator_category type2;
    return __copy(first, last, dest, type1(), type2());
}

template <class T>
T* __copy_dispatch(T *first, T *last, T *dest) {
    typedef typename hh_std::__type_traits<T>::has_trivial_assignment_operator type;
    return __copy_t(first, last, dest, type());
}

template <class T>
T* __copy_dispatch(const T *first, const T *last, T *dest) {
    typedef typename hh_std::__type_traits<T>::has_trivial_assignment_operator type;
    return __copy_t(first, last, dest, type());
}

template <class Iter1, class Iter2>
Iter2 copy(Iter1 first, Iter1 last, Iter2 dest) {
    //typedef typename hh_std::__type_traits <typename Iter2::value_type>::has_trivial_assignment_operator type;
    return __copy_dispatch(first, last, dest);
}

template <>
char* copy(const char* first, const char* last, char* dest) {
    std::memmove(dest, first, (last - first));
    return dest + (last - first);
}

template <>
wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* dest) {
    std::memmove(dest, first, sizeof(wchar_t) * (last - first));
    return dest + (last - first);
}

template <class BidirectIter1, class BidirectIter2>
BidirectIter2 __copy_backward_d(BidirectIter1 first, BidirectIter1 last, BidirectIter2 end) {
    ptrdiff_t len = last - first;
    BidirectIter2 start = end - len;
    while(len > 0) {
        --len;
        start[len] = first[len];
    }
    return start;
}

template <class BidirectIter1, class BidirectIter2>
BidirectIter2 __copy_backward(BidirectIter1 first, BidirectIter1 last, BidirectIter2 end, random_access_iterator_tag, random_access_iterator_tag) {
    return __copy_backward_d(first, last, end);
}

template <class BidirectIter1, class BidirectIter2>
BidirectIter2 __copy_backward(BidirectIter1 first, BidirectIter1 last, BidirectIter2 end, input_iterator_tag, input_iterator_tag) {
    while(first != last) {
        --last;
        --end;
        *end = *last;
    }
    return end;
}

template <class BidirectIter1, class BidirectIter2>
BidirectIter2 __copy_backward_dispatch(BidirectIter1 first, BidirectIter1 last, BidirectIter2 end) {
    typedef typename BidirectIter1::iterator_category type1;
    typedef typename BidirectIter2::iterator_category type2;
    return __copy_backward(first, last, end, type1(), type2());
}

template <class T>
T *__copy_backward_t(const T *first, const T *last, T *end, __true_type) {
    std::memmove(end - (last - first), first, sizeof(T) * (last - first));
    return end - (last - first);
}

template <class T>
T *__copy_backward_t(const T *first, const T *last, T *end, __false_type) {
    return __copy_backward_d(first, last, end);
}

template <class T>
T *__copy_backward_dispatch(T *first, T *last, T *end) {
    typedef typename hh_std::__type_traits<T>::has_trivial_assignment_operator type;
    return __copy_backward_t(first, last, end, type());
}

template <class T>
T *__copy_backward_dispatch(const T *first, const T *last, T *end) {
    typedef typename hh_std::__type_traits<T>::has_trivial_assignment_operator type;
    return __copy_backward_t(first, last, end, type());
}

template <class BidirectIter1, class BidirectIter2>
BidirectIter2 copy_backward(BidirectIter1 first, BidirectIter1 last, BidirectIter2 end) {
    return __copy_backward_dispatch(first, last, end);
}

template <>
char* copy_backward(const char *first, const char *last, char* end) {
    std::memmove(end - (last - first), first, (last - first));
    return end - (last - first);
}

template <>
wchar_t* copy_backward(const wchar_t *first, const wchar_t *last, wchar_t *end) {
    std::memmove(end - (last - first), first, sizeof(wchar_t) * (last - first));
    return end - (last - first);
}


};

#endif
