#ifndef __HH_STL_CONSTRUCT_H_
#define __HH_STL_CONSTRUCT_H_

#include<new>
#include "hh_type_traits.h"
#include "hh_stl_iterator.h"

template <class Point, class T>
inline void construct(Point *p, const T &value) {
    new (p) T((T&)value);
}

template <class T>
inline void destory(T *p) {
    p->~T();
}

template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, hh_std::__false_type) {
    while(first != last) {
        destory(&*first);
        first++;
    }
}

template <class ForwardIterator>
inline void __destory_aux(ForwardIterator first, ForwardIterator last, hh_std::__true_type) {

}

template <class ForwardIterator, class T>
inline void __destory(ForwardIterator first, ForwardIterator last, T *) {
    typedef typename hh_std::__type_traits<T>::has_trivial_destructor destructor;
    __destory_aux(first, last, destructor());
}

template <class ForwardIterator>
inline void destory(ForwardIterator first, ForwardIterator last ) {
    __destory(first, last, hh_std::value_type(first));
}

template <>
inline void destory(char*, char*) {

}

template <>
void destory(wchar_t*, wchar_t*) {

}

#endif
