#ifndef __HH_STL_QUEUE_H_
#define __HH_STL_QUEUE_H_

#include "hh_stl_deque.h"

namespace hh_std {

template <class T, class Sequence>
class queue;

template <class T, class Sequence>
bool operator== (const queue<T, Sequence> &x, const queue<T, Sequence> &y) {
    return x == y;
}

template <class T, class Sequence>
bool operator< (const queue<T, Sequence> &x, const queue<T, Sequence> &y) {
    return x < y;
}

template <class T, class Sequence = deque<T>>
class queue {
    friend bool operator== <> (const queue &x, const queue &y);
    friend bool operator< <> (const queue &x, const queue &y);
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
        return c.front();
    }
    const_reference top() const {
        return c.front();
    }
    reference back() {
        return c.back();
    }
    const_reference back() const {
        return c.back();
    }
    void pop() {
        c.pop_front();
    }
    void push(const value_type &x) {
        c.push_back(x);
    }
};

};

#endif
