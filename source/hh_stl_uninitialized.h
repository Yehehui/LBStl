#ifndef __HH_STL_UNINITIALIZED_H_
#define __HH_STL_UNINITIALIZED_H_

#include "hh_stl_iterator.h"
#include "hh_type_traits.h"
#include "hh_stl_construct.h"
#include<cstring>
#include<algorithm>

template <class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, hh_std::__true_type) {
    return std::copy(first, last, dest);
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator dest, hh_std::__false_type) {
    while(first != last) {
        construct(&*dest, *first);
        first++;
        dest++;
    }
    return dest;
}

template <class InputIterator, class ForwardIterator, class T>
inline ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest, T*) {
    typedef typename hh_std::__type_traits<T>::is_POD_type type;
    return __uninitialized_copy_aux(first, last, dest, type());
}

template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator dest) {
    return __uninitialized_copy(first, last, dest, hh_std::value_type(first));
}

template <>
inline char *uninitialized_copy(char *first, char *last, char *dest) {
    memmove(dest, first, last - first);
    return dest + (last - first);
}

template <>
inline wchar_t *uninitialized_copy(wchar_t *first, wchar_t *last, wchar_t *dest) {
    memmove(dest, first, last - first);
    return dest + (last - first);
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, T &value, hh_std::__true_type) {
    std::fill(first, last, value);
}

template <class ForwardIterator, class T>
inline void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, T &value, hh_std::__false_type) {
    while(first != last) {
        construct(&*first, value);
        first++;
    }
}

template <class ForwardIterator, class T1, class T2>
inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, T1 &value, T2 *) {
    typedef typename hh_std::__type_traits<T2>::is_POD_type type;
    __uninitialized_fill_aux(first, last, value, type());
}

template <class ForwardIterator, class T>
inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, T &value) {
    __uninitialized_fill(first, last, value, hh_std::value_type(first));
}

template <class ForwardIterator, class Size, class T>
inline void __uninitialized_fill_n_aux(ForwardIterator first, Size n, T &value, hh_std::__true_type) {
    std::fill_n(first, n, value);
}

template <class ForwardIterator, class Size, class T>
inline void __uninitialized_fill_n_aux(ForwardIterator first, Size n, T &value, hh_std::__false_type) {
    while(n > 0) {
        construct(&*first, value);
        first++;
        n--;
    }
}

template <class ForwardIterator, class Size, class T, class T2>
inline void __uninitialized_fill_n(ForwardIterator first, Size n, T &value, T2 *) {
    typedef typename hh_std::__type_traits<T2>::is_POD_type type;
    __uninitialized_fill_n_aux(first, n, value, type());
}

template <class ForwardIterator, class Size, class T>
inline void uninitialized_fill_n(ForwardIterator first, Size n, T &value) {
    __uninitialized_fill_n(first, n, value, value_type(first));
}

#endif
