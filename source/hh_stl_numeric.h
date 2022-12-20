#ifndef __HH_STL_NUMERIC_H_
#define __HH_STL_NUMERIC_H_

#include "hh_stl_iterator.h"

namespace hh_std {

template <class Arg, class Result>
struct unary_function {
    typedef Arg first_argument_type;
    typedef Result result_type;
};

template <class Arg1, class Arg2, class Result>
struct binary_function {
    typedef Arg1 first_argument_type;
    typedef Arg2 second_argument_type;
    typedef Result result_type;
};

template <class T>
struct plus: public binary_function<T, T, T> {
    T operator() (const T &x, const T &y) const {
        return x + y;
    }
};

template <class T>
struct minus: public binary_function<T, T, T> {
    T operator() (const T &x, const T &y) const {
        return x - y;
    }
};

template <class T>
struct multiplies: public binary_function<T, T, T> {
    T operator() (const T &x, const T &y) const {
        return x * y;
    }
};

template <class T>
struct divides: public binary_function<T, T, T> {
    T operator() (const T &x, const T &y) const {
        return x / y;
    }
};

template <class T>
struct modulus: public binary_function<T, T, T> {
    T operator() (const T &x, const T &y) const {
        return x % y;
    }
};

template <class T>
struct negate: public unary_function<T, T> {
    T operator() (const T &x) const {
        return -x;
    }
};

template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T begin) {
    for(; first != last; ++first) {
        begin = begin + *first;
    }
    return begin;
}

template <class InputIterator, class T, class BinaryOps>
T accumulate(InputIterator first, InputIterator last, T begin, BinaryOps bo = plus<int>()) {
    for(; first != last; ++first) {
        begin = bo(begin, *first);
    }
    return begin;
}

template <class Iter1, class Iter2, class T>
T inner_product(Iter1 first, Iter1 last, Iter2 begin, T init) {
    for(; first != last; ++begin, ++first) {
        init = init + (*first) * (*begin);
    }
    return init;
}
template <class Iter1, class Iter2, class T, class BinaryOps1, class BinaryOps2>
T inner_product(Iter1 first, Iter1 last, Iter2 begin, T init, BinaryOps1 op1, BinaryOps2 op2) {
    for(; first != last; ++first, ++begin) {
        init = op2(init, op1(*first, *begin));
    }
    return init;
}

template <class InputIterator, class OutputIterator>
void partial_sum(InputIterator first, InputIterator last, OutputIterator output) {
    if(first != last) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        type init = *first;
        *output = init;
        ++first;
        for(; first != last; ++first) {
            init = init + (*first);
            *output = init;
        }
    }
}

template <class InputIterator, class OutputIterator, class BinaryOps>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator output, BinaryOps op) {
    if(first != last) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        type init = *first;
        *output = init;
        ++first;
        for(; first != last; ++first) {
            init = op(init, *first);
            *output = init;
        }
    }
    return output;
}

template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator output) {
    if(first != last) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        *output = *first;
        type pre = *first;
        ++first;
        for(; first != last; ++first) {
            *output = *first - pre;
            pre = *first;
        }
    }
    return output;
}

template <class InputIterator, class OutputIterator, class BinaryOps>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator output, BinaryOps op) {
    if(first != last) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        *output = *first;
        type pre = *first;
        ++first;
        for(; first != last; ++first) {
            *output = op(*first, pre);
            pre = *first;
        }
    }
    return output;
}

template <class Value>
Value power(Value val, unsigned int times) {
    if(times == 0) return 1;
    Value result = val;
    while(times > 1) {
        result *= val;
        --times;
    }
    return result;
}

template <class Value, class BinaryOps>
Value power(Value val, unsigned int times, BinaryOps op) {
    if(times == 0) return 1;
    Value result = val;
    while(times > 1) {
        result = op(result, val);
        --times;
    }
    return result;
}

template <class InputIterator>
void itoa(InputIterator first, InputIterator last, int init) {
    for(; first != last; ++first) {
        *first = init;
        ++init;
    }
}

};

#endif
