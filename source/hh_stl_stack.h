#ifndef __HH_STL_STACK_H_
#define __HH_STL_STACK_H_

#include "hh_stl_deque.h"

namespace hh_std {

template<class T, class Sequence>
class stack;

template <class T, class Sequence>
bool operator== (const stack<T, Sequence> &x, const stack<T, Sequence> &y) {
    return x == y;
}

template <class T, class Sequence>
bool operator< (const stack<T, Sequence> &x, const stack<T, Sequence> &y) {
    return x < y;
}

template <class T, class Sequence = deque<T>>
class stack {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c;
public:
    size_type size() const {
        return c.size();
    }
    bool empty() const {
        return c.empty();
    }
    reference top() {
        return c.back();
    }
    const_reference top() const {
        return c.back();
    }
    void pop() {
        c.pop_back();
    }
    void push(const value_type &x) {
        c.push_back(x);
    }
    //template <class T1, class Sequence1>
    friend bool operator== <> (const stack &x, const stack &y);
    //template <class T1, class Sequence1>
    friend bool operator< <> (const stack &x, const stack &y);
};

}
#endif
