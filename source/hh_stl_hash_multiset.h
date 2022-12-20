#ifndef __HH_STL_HASH_MULTISET_H_
#define __HH_STL_HASH_MULTISET_H_

#include "hh_stl_alloc.h"
#include "hh_stl_hash_fun.h"
#include "hh_stl_hashtable.h"
#include "hh_stl_functional.h"
#include<cstdlib>
#include<algorithm>

namespace hh_std {

template <class Value, class HashFun, class EqualFun, class Alloc>
struct hash_multiset;

template <class Value, class HashFun, class EqualFun, class Alloc>
inline bool operator== (const hash_multiset<Value, HashFun, EqualFun, Alloc> &x, const hash_multiset<Value, HashFun, EqualFun, Alloc> &y) {
    return x.ht == y.ht;
}

template < class Value, class HashFun = hh_std::hash<Value>, class EqualFun = std::equal_to<Value>, class Alloc = hh_std::alloc>
struct hash_multiset {
private:
    typedef hh_std::hashtable < Value, Value, hh_std::identity<Value>, HashFun, EqualFun, Alloc> rep_type;
    rep_type ht;
public:
    typedef typename rep_type::key_type key_type;
    typedef typename rep_type::value_type value_type;
    typedef typename rep_type::hasher hasher;
    typedef typename rep_type::key_equal equals;

    typedef typename rep_type::const_pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::const_reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::difference_type difference_type;
    typedef typename rep_type::size_type size_type;

    typedef typename rep_type::const_iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;

    hasher hash_fun() const {
        return hasher();
    }
    equals eq_fun() const {
        return equals();
    }

public:
    hash_multiset(): ht(50, hash_fun(), eq_fun()) {}
    explicit hash_multiset(const size_type n): ht(n, hash_fun(), eq_fun()) {}
    hash_multiset(const size_type n, const hasher &h): ht(n, h, eq_fun()) {}
    hash_multiset(const size_type n, const hasher &h, const equals &e): ht(n, h, e) {}

    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last): ht(50, hash_fun(), eq_fun()) {
        ht.insert_equal(first, last);
    }
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, const size_type n): ht(n, hash_fun(), eq_fun()) {
        ht.insert_equal(first, last);
    }
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, const size_type n, const hasher &h): ht(n, h, eq_fun()) {
        ht.insert_equal(first, last);
    }
    template <class InputIterator>
    hash_multiset(InputIterator first, InputIterator last, const size_type n, const hasher &h, const equals &e): ht(n, h, e) {
        ht.insert_equal(first, last);
    }
    size_type size() const {
        return ht.size();
    }
    bool empty() const {
        return ht.empty();
    }
    size_type max_size() const {
        return ht.max_size();
    }
    void swap(hash_multiset &h) {
        ht.swap(h.ht);
    }
    iterator begin() const {
        return ht.begin();
    }
    iterator end() const {
        return ht.end();
    }
    iterator insert(const value_type &x) {
        typename rep_type::iterator result = ht.insert_equal(x);
        return iterator(result.cur, result.table);
    }
    iterator insert_noresize(const value_type &x) {
        typename rep_type::iterator result = ht.insert_equal_noresize(x);
        return iterator(result.cur, result.table);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        ht.insert_equal(first, last);
    }
    iterator find(const key_type &k) const {
        return ht.find(k);
    }
    size_type count(const key_type &k) const {
        return ht.count(k);
    }
    hh_std::pair<iterator, iterator> equal_range(const key_type &k) const {
        return ht.equal_range(k);
    }
    size_type erase(const key_type &k) {
        return ht.erase(k);
    }
    void erase(iterator it) {
        ht.erase(it);
    }
    void erase(iterator first, iterator last) {
        ht.erase(first, last);
    }
    void clear() {
        ht.clear();
    }
    void resize(const size_type n) {
        ht.resize(n);
    }
    size_type bucket_count() const {
        return ht.bucket_count();
    }
    size_type max_bucket_count() const {
        return ht.max_bucket_count();
    }
    size_type elem_in_bucket(const size_type n) const {
        return ht.elem_in_bucket(n);
    }
    friend bool operator== <> (const hash_multiset &x, const hash_multiset&y);
};

template <class Value, class HashFun, class EqualFun, class Alloc>
inline void swap(hash_multiset<Value, HashFun, EqualFun, Alloc> &x, hash_multiset<Value, HashFun, EqualFun, Alloc> &y) {
    x.swap(y);
}

};

#endif
