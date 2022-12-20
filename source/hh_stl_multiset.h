#ifndef __HH_STL_MULTISET_H_
#define __HH_STL_MULTISET_H_

#include "hh_stl_alloc.h"
#include "hh_stl_tree.h"
#include "hh_stl_functional.h"

namespace hh_std {

template <class Key, class Compare, class Alloc>
class multiset;

template <class Key, class Compare, class Alloc>
bool operator== (const multiset<Key, Compare, Alloc> &x, const multiset<Key, Compare, Alloc> &y) {
    return x.t == y.t;
}

template <class Key, class Compare, class Alloc>
bool operator< (const multiset<Key, Compare, Alloc> &x, const multiset<Key, Compare, Alloc> &y) {
    return x.t < y.t;
}

template <class Key, class Compare = std::less<Key>, class Alloc = alloc>
class multiset {
public:
    typedef Key key_type;
    typedef Key value_type;
    typedef Compare key_compare;
    typedef Compare value_compare;
private:
    typedef hh_std::rb_tree<key_type, value_type, hh_std::identity<value_type>, Compare, Alloc> rep_type;
    rep_type t;
public:
    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::difference_type difference_type;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::const_iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::const_r_iterator r_iterator;
    typedef typename rep_type::const_r_iterator const_r_iterator;
    multiset(): t(Compare()) {}
    explicit multiset(const Compare &comp): t(comp) {}
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last): t(Compare()) {
        t.insert_equal(first, last);
    }
    template <class InputIterator>
    multiset(InputIterator first, InputIterator last, const Compare &comp): t(comp) {
        t.insert_equal(first, last);
    }
    multiset(const multiset<Key, Compare, Alloc> &x): t(x.t) {}
    multiset<Key, Compare, Alloc> &operator= (const multiset<Key, Compare, Alloc> &x) {
        t = x.t;
        return *this;
    }
    key_compare key_comp() const {
        return t.comp;
    }
    value_compare value_comp() const {
        return t.comp;
    }
    iterator begin() const {
        return t.begin();
    }
    iterator end() const {
        return t.end();
    }
    r_iterator rbegin() const {
        return t.rbegin();
    }
    r_iterator rend() const {
        return t.rend();
    }
    size_type size() const {
        return t.size();
    }
    bool empty() const {
        return t.empty();
    }
    size_type max_size() const {
        return t.max_size();
    }
    void swap(multiset<Key, Compare, Alloc> &x) {
        t.swap(x.t);
    }
    iterator insert(const value_type &x) {
        typename rep_type::iterator result = t.insert_equal(x);
        return iterator(result.node);
    }
    iterator insert(iterator pos, const value_type &x) {
        return t.insert((typename rep_type::iterator)pos, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t.insert_equal(first, last);
    }
    void erase(iterator pos) {
        t.erase(typename rep_type::iterator(pos.node));
    }
    size_type erase(const key_type &x) {
        return t.erase(x);
    }
    void erase(iterator first, iterator last) {
        t.erase((typename rep_type::iterator)first.node, (typename rep_type::iterator)last.node);
    }
    void clear() {
        t.clear();
    }
    iterator find(const key_type &x) const {
        return t.find(x);
    }
    size_type count(const key_type &x) const {
        return t.count(x);
    }
    iterator lower_bound(const key_type &x) const {
        return t.lower_bound(x);
    }
    iterator upper_bound(const key_type &x) const {
        return t.upper_bound(x);
    }
    hh_std::pair<iterator, iterator> equal_range(const key_type &x) const {
        return t.equal_range(x);
    }
    friend bool operator== <> (const multiset<Key, Compare, Alloc> &x, const multiset<Key, Compare, Alloc> &y);
    friend bool operator< <> (const multiset<Key, Compare, Alloc> &x, const multiset<Key, Compare, Alloc> &y);
};


template <class Key, class Compare, class Alloc>
inline void swap(multiset<Key, Compare, Alloc> &x, multiset<Key, Compare, Alloc> &y) {
    x.swap(y);
}

};

#endif
