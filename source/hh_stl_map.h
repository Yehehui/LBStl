#ifndef __HH_STL_MAP_H_
#define __HH_STL_MAP_H_

#include "hh_stl_alloc.h"
#include "hh_stl_tree.h"
#include "hh_stl_pair.h"

namespace hh_std {


template <class Key, class T, class Compare, class Alloc>
class map;


template <class Key, class T, class Compare, class Alloc>
inline bool operator== (const map<Key, T, Compare, Alloc> &x, const map<Key, T, Compare, Alloc> &y) {
    return x.t == y.t;
}

template <class Key, class T, class Compare, class Alloc>
inline bool operator< (const map<Key, T, Compare, Alloc> &x, const map<Key, T, Compare, Alloc> &y) {
    return x.t < y.t;
}

template <class Key, class T, class Compare = std::less<Key>, class Alloc = alloc>
class map {
public:
    typedef Key key_type;
    typedef T data_type;
    typedef hh_std::pair<const key_type, data_type> value_type;
    typedef Compare key_compare;
    class value_compare: public std::binary_function<value_type, value_type, bool> {
    private:
        Compare comp;
    public:
        value_compare(Compare &c): comp(c) {}
        bool operator() (const value_type &x, const value_type &y) const {
            return comp(x.first, y.first);
        }
    };
private:
    typedef hh_std::rb_tree<value_type, key_type, hh_std::select1st<value_type>, Compare, Alloc> rep_type;
    rep_type t;
public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;
    typedef typename rep_type::r_iterator r_iterator;
    typedef typename rep_type::const_r_iterator const_r_iterator;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;

    map(): t(Compare()) {}
    explicit map(Compare &c): t(c) {}
    template <class InputIterator>
    map(InputIterator first, InputIterator last): t(Compare()) {
        t.insert_unique(first, last);
    }
    template <class InputIterator>
    map(InputIterator first, InputIterator last, Compare &c): t(c) {
        t.insert_unique(first, last);
    }
    map(const map<Key, T, Compare, Alloc> &x): t(x.t) {}
    map<Key, T, Compare, Alloc> &operator= (const map<Key, T, Compare, Alloc> &x) {
        t = x.t;
        return *this;
    }
    key_compare key_comp() const {
        return t.key_comp();
    }
    value_compare value_comp() const {
        return value_compare(t.key_comp());
    }
    iterator begin() {
        return t.begin();
    }
    const_iterator begin() const {
        return t.begin();
    }
    iterator end() {
        return t.end();
    }
    const_iterator end() const {
        return t.end();
    }
    r_iterator rbegin() {
        return t.rbegin();
    }
    const_r_iterator rbegin() const {
        return t.rbegin();
    }
    r_iterator rend() {
        return t.rend();
    }
    const_r_iterator rend() const {
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
    T &operator[] (const key_type &k) {
        return (*(insert(value_type(k, T())).first)).second;
    }
    void swap(const map<Key, T, Compare, Alloc> &x) {
        t.swap(x.t);
    }
    hh_std::pair<iterator, bool> insert(const value_type &x) {
        return t.insert_unique(x);
    }
    iterator insert(iterator pos, const value_type &x) {
        return t.insert_unique(pos, x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        t.insert_unique(first, last);
    }
    void erase(iterator pos) {
        t.erase(pos);
    }
    size_type erase(const key_type &k) {
        return t.erase(k);
    }
    void erase(iterator first, iterator last) {
        t.erase(first, last);
    }
    void clear() {
        t.clear();
    }
    iterator find(const key_type &k) {
        return t.find(k);
    }
    const_iterator find(const key_type &k) const {
        return t.find(k);
    }
    size_type count(const key_type &k) const {
        return t.count(k);
    }
    iterator lower_bound(const key_type &k) {
        return t.lower_bound(k);
    }
    const_iterator lower_bound(const key_type &k) const {
        return t.lower_bound(k);
    }
    iterator upper_bound(const key_type &k) {
        return t.upper_bound(k);
    }
    const_iterator upper_bound(const key_type &k) const {
        return t.upper_bound(k);
    }
    hh_std::pair<iterator, iterator> equal_range(const key_type &k) {
        return t.equal_range(k);
    }
    hh_std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
        return t.equal_range(k);
    }
    friend bool operator== <> (const map &x, const map &y);
    friend bool operator< <> (const map &x, const map &y);
};

template <class Key, class T, class Compare, class Alloc>
inline void swap(map<Key, T, Compare, Alloc> &x, map<Key, T, Compare, Alloc> &y) {
    x.swap(y);
}

};

#endif
