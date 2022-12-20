#ifndef __HH_STL_PAIR_H_
#define __HH_STL_PAIR_H_

#include "hh_stl_numeric.h"

namespace hh_std {

template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;
    T1 first;
    T2 second;
    pair(): first(T1()), second(T2()) {}
    pair(const T1 &x, const T2 &y): first((T1&)x), second(y) {}
    pair(const pair<T1, T2> &x): first(x.first), second(x.second) {}
};

template <class T1, class T2>
inline bool operator== (pair<T1, T2> &x, pair<T1, T2> &y) {
    return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
inline bool operator< (pair<T1, T2> &x, pair<T1, T2> &y) {
    return x.first < y.first ? true : (x.first == y.first ? x.second < y.second : false);
}

template <class T1, class T2>
pair<T1, T2> make_pair(const T1 &x, const T2 &y) {
    return pair<T1, T2>(x, y);
}

template <class T>
struct select1st: public hh_std::unary_function < T, typename T::first_type> {
    typename T::first_type &operator() (const T &p) const {
        return (typename T::first_type&)p.first;
    }
};

template <class T>
struct select2nd: public hh_std::unary_function < T, typename T::second_type> {
    typename T::second_type &operator() (const T &p) const {
        return (typename T::second_type&)p.first;
    }
};

template <class Arg1, class Arg2>
struct project1st: public hh_std::binary_function<Arg1, Arg2, Arg1> {
    Arg1 operator() (const Arg1 &x, const Arg2 &) const {
        return x;
    }
};

template <class Arg1, class Arg2>
struct project2nd: public hh_std::binary_function<Arg1, Arg2, Arg2> {
    Arg2 operator() (const Arg1 &, const Arg2 &y) const {
        return y;
    }
};

};

#endif
