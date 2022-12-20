#ifndef __HH_STL_PRIORITY_QUEUE_H_
#define __HH_STL_PRIORITY_QUEUE_H_

#include "hh_stl_vector.h"
#include "hh_stl_heap.h"

namespace hh_std {

template <class T, class Sequence = hh_std::vector<T>, class Compare = std::less<typename Sequence::value_type>>
class priority_queue {
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;
    typedef typename Sequence::const_reference const_reference;
protected:
    Sequence c;
    Compare comp;
public:
    priority_queue(): c() {}
    explicit priority_queue(Compare cp): c(), comp(cp) {}
    template <class InputIterator, class Compare1>
    priority_queue(InputIterator first, InputIterator last, Compare1 cp): c(first, last), comp(cp) {
        hh_std::make_heap(c.begin(), c.end(), comp);
    }
    template <class InputIterator>
    priority_queue(InputIterator first, InputIterator last): c(first, last) {
        hh_std::make_heap(c.begin(), c.end(), comp);
    }
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
    void push(const value_type &x) {
        c.push_back(x);
        hh_std::push_heap(c.begin(), c.end(), comp);
    }
    void pop() {
        hh_std::pop_heap(c.begin(), c.end(), comp);
        c.pop_back();
    }
};

};

#endif
