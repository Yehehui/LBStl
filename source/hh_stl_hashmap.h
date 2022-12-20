#ifndef __HH_STL_HASHMAP_H_
#define __HH_STL_HASHMAP_H_

#include "hh_stl_hashtable.h"
#include "hh_stl_pair.h"
#include "hh_stl_alloc.h"

namespace hh_std {

template <class Key, class T, class HashFun, class EqualFun, class Alloc>
struct hash_map;

template <class Key, class T, class HashFun, class EqualFun, class Alloc>
inline bool operator== (const hash_map<Key, T, HashFun, EqualFun, Alloc> &x, const hash_map<Key, T, HashFun, EqualFun, Alloc> &y) {
    return x.ht == y.ht;
}

template<class Key, class T, class HashFun = hh_std::hash<Key>, class EqualFun = std::equal_to<Key>, class Alloc = hh_std::alloc>
struct hash_map {
public:
    typedef Key key_type;
    typedef T data_type;
    typedef hh_std::pair<Key, T> value_type;
    typedef HashFun hasher;
    typedef EqualFun key_equal;

private:
    typedef hh_std::hashtable<key_type, value_type, hh_std::select1st<value_type>, HashFun, EqualFun, Alloc> rep_type;
    rep_type ht;

public:
    typedef typename rep_type::pointer pointer;
    typedef typename rep_type::const_pointer const_pointer;
    typedef typename rep_type::reference reference;
    typedef typename rep_type::const_reference const_reference;
    typedef typename rep_type::size_type size_type;
    typedef typename rep_type::difference_type difference_type;
    typedef typename rep_type::iterator iterator;
    typedef typename rep_type::const_iterator const_iterator;

    hasher hash_fun() const {
        return hasher();
    }
    key_equal key_eq() const {
        return key_equal();
    }

    hash_map(): ht(50, hash_fun(), key_eq()) {}
    explicit hash_map(const size_type n): ht(n, hash_fun(), key_eq()) {}
    hash_map(const size_type n, const hasher &h): ht(n, h, key_eq()) {}
    hash_map(const size_type n, const hasher &h, const key_equal &k): ht(n, h, k) {}

    template <class InputIterator>
    hash_map(InputIterator first, InputIterator last): ht(50, hash_fun(), key_eq()) {
        ht.insert_unique(first, last);
    }
    template <class InputIterator>
    hash_map(InputIterator first, InputIterator last, const size_type n): ht(n, hash_fun(), key_eq()) {
        ht.insert_unique(first, last);
    }
    template <class InputIterator>
    hash_map(InputIterator first, InputIterator last, const size_type n, const hasher &h): ht(n, h, key_eq()) {
        ht.insert_unique(first, last);
    }
    template <class InputIterator>
    hash_map(InputIterator first, InputIterator last, const size_type n, const hasher &h, const key_equal &k): ht(n, h, k) {
        ht.insert_unique(first, last);
    }

    size_type size() const {
        return ht.size();
    }
    bool empty() const {
        return ht.empty();
    }
    size_type max_size() const {
        return size_type(-1);
    }
    void swap(hash_map<Key, T, HashFun, EqualFun, Alloc> &x) {
        ht.swap(x.ht);
    }
    iterator begin() {
        return ht.begin();
    }
    const_iterator begin() const {
        return ht.begin();
    }
    iterator end() {
        return ht.end();
    }
    const_iterator end() const {
        return ht.end();
    }
    data_type &operator[] (const key_type &k) {
        return ht.find_or_insert(value_type(k, T())).second;
    }
    hh_std::pair<iterator, bool> insert(const value_type &x) {
        return ht.insert_unique(x);
    }
    hh_std::pair<iterator, bool> insert_noresize(const value_type &x) {
        return ht.insert_unique_noresize(x);
    }
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last) {
        ht.insert_unique(first, last);
    }
    iterator find(const key_type &k) {
        return ht.find(k);
    }
    const_iterator find(const key_type &k) const {
        return ht.find(k);
    }
    size_type count(const key_type &k) const {
        return ht.count(k);
    }
    hh_std::pair<iterator, iterator> equal_range(const key_type &k) {
        return ht.equal_range(k);
    }
    hh_std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
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
    size_type elem_in_bucket(size_type n) const {
        return ht.elem_in_bucket(n);
    }
    friend bool operator== <> (const hash_map &x, const hash_map &y);
};

template <class Key, class T, class HashFun, class EqualFun, class Alloc>
inline void swap(hash_map<Key, T, HashFun, EqualFun, Alloc> &x, hash_map<Key, T, HashFun, EqualFun, Alloc> &y) {
    x.swap(y);
}

};

#endif
