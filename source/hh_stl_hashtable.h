#ifndef __HH_STL_HASHTABLE_H_
#define __HH_STL_HASHTABLE_H_

#include "hh_stl_alloc.h"
#include "hh_stl_iterator.h"
#include "hh_stl_vector.h"
#include "hh_config.h"
#include "hh_stl_construct.h"
#include "hh_stl_pair.h"
#include "hh_stl_hash_fun.h"

namespace hh_std {

template <class Value>
struct __hashtable_node {
    __hashtable_node* next;
    Value data;
};

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc>
struct hashtable;

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc>
struct __hashtable_iterator;

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc>
struct __hashtable_const_iterator;

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc>
struct __hashtable_iterator {
public:
    typedef hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> _hashtable;
    typedef __hashtable_iterator<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> iterator;
    typedef __hashtable_const_iterator<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> const_iterator;
    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef Value* pointer;
    typedef Value& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    node* cur;
    _hashtable* table;

    __hashtable_iterator() = default;
    __hashtable_iterator(node* n, _hashtable* t): cur(n), table(t) {}

    reference operator* () const {
        return cur->data;
    }
    pointer operator-> () const {
        return &(operator*());
    }
    iterator &operator++ () {
        if(cur->next) {
            cur = cur->next;
        } else {
            size_type pos = table->bkt_num(cur->data) + 1;
            for(; pos < table->buckets.size(); ++pos) {
                if(table->buckets[pos]) {
                    cur = table->buckets[pos];
                    break;
                }
            }
            if(pos == table->buckets.size())
                cur = 0;
        }
        return *this;
    }
    iterator operator++ (int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
    bool operator== (const iterator &x) const {
        return cur == x.cur;
    }
    bool operator!= (const iterator &x) const {
        return cur != x.cur;
    }
};

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc>
struct __hashtable_const_iterator {
public:
    typedef hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> _hashtable;
    typedef __hashtable_iterator<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> iterator;
    typedef __hashtable_const_iterator<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> const_iterator;
    typedef __hashtable_node<Value> node;

    typedef forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef const Value* pointer;
    typedef const Value& reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    const node* cur;
    const _hashtable* table;

    __hashtable_const_iterator() = default;
    __hashtable_const_iterator(const node* n, const _hashtable* t): cur(n), table(t) {}

    reference operator* () const {
        return cur->data;
    }
    pointer operator-> () const {
        return &(operator*());
    }
    const_iterator &operator++ () {
        if(cur->next) {
            cur = cur->next;
        } else {
            size_type pos = table->bkt_num(cur->data) + 1;
            for(; pos < table->buckets.size(); ++pos) {
                if(table->buckets[pos]) {
                    cur = table->buckets[pos];
                    break;
                }
            }
            if(pos == table->buckets.size())
                cur = 0;
        }
        return *this;
    }
    const_iterator operator++ (int) {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }
    bool operator== (const const_iterator &x) const {
        return cur == x.cur;
    }
    bool operator!= (const const_iterator &x) const {
        return cur != x.cur;
    }
};

static const int __stl_num_primes = 28;
static const unsigned long __stl_prime_list[__stl_num_primes] = {
    53,         97,           193,         389,       769,
    1543,       3079,         6151,        12289,     24593,
    49157,      98317,        196613,      393241,    786433,
    1572869,    3145739,      6291469,     12582917,  25165843,
    50331653,   100663319,    201326611,   402653189, 805306457,
    1610612741, 3221225473ul, 4294967291ul
};

inline unsigned long __stl_next_prime(unsigned long n) {
    for(size_t i = 0; i < __stl_num_primes; ++i) {
        if(__stl_prime_list[i] >= n)
            return __stl_prime_list[i];
    }
    return __stl_prime_list[__stl_num_primes - 1];
}

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc = alloc>
inline bool operator== (const hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> &x, const hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> &y) {
    if(x.size() != y.size() || x.bucket_count() != y.bucket_count()) return false;
    typedef typename hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc>::node node;
    for(size_t i = 0; i < x.bucket_count(); ++i) {
        node *n1 = x.buckets[i], *n2 = y.buckets[i];
        for(; n1 && n2 && n1->data == n2->data; n1 = n1->next, n2 = n2->next) { }
        if(n1 || n2) return false;
    }
    return true;
}

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc = alloc>
inline void swap (const hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> &x, const hashtable<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> &y) {
    x.swap(y);
}

template <class Key, class Value, class KeyofValue, class HashFun, class EqualFun, class Alloc = alloc>
struct hashtable {
public:
    typedef Key key_type;
    typedef Value value_type;
    typedef HashFun hasher;
    typedef EqualFun key_equal;

    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

    hasher hash_func() const {
        return hasher();
    }
    key_equal key_eq() const {
        return key_equal();
    }

private:
    hasher hash;
    key_equal equals;
    KeyofValue kov;

    typedef __hashtable_node<Value> node;
    typedef hh_std::simple_alloc<node, Alloc> node_allocater;

    hh_std::vector<node*, Alloc> buckets;
    size_type num_elements;

public:
    typedef __hashtable_iterator<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> iterator;
    typedef __hashtable_const_iterator<Key, Value, KeyofValue, HashFun, EqualFun, Alloc> const_iterator;
    friend iterator;
    friend const_iterator;

    hashtable(size_type n, const KeyofValue &k, const HashFun &h, const EqualFun &e): kov(k), hash(h), equals(e) {
        initialize_buckets(n);
    }
    hashtable(size_type n, const HashFun &h, const EqualFun &e): kov(KeyofValue()), hash(h), equals(e) {
        initialize_buckets(n);
    }
    hashtable(const hashtable &h): kov(h.kov), hash(h.hash), equals(h.equals), num_elements(0) {
        copy_from(h);
    }
    hashtable &operator= (const hashtable &h) {
        if(this != &h) {
            clear();
            hash = h.hash;
            equals = h.equals;
            kov = h.kov;
            copy_from(h);
        }
        return *this;
    }
    ~hashtable() {
        clear();
    }
    size_type size() const {
        return num_elements;
    }
    size_type max_size() const {
        return size_type(-1);
    }
    bool empty() const {
        return num_elements == 0;
    }
    void swap(hashtable &h) {
        swap(kov, h.kov);
        swap(equals, h.equals);
        swap(hash, h, hash);
        swap(num_elements, h.num_elements);
        swap(buckets, h.buckets);
    }
    iterator begin() {
        for(size_type i = 0; i < buckets.size(); ++i) {
            if(buckets[i])
                return iterator(buckets[i], this);
        }
        return iterator((node*)0, this);
    }
    const_iterator begin() const {
        for(size_type i = 0; i < buckets.size(); ++i) {
            if(buckets[i])
                return const_iterator(buckets[i], this);
        }
        return const_iterator((node*)0, this);
    }
    iterator end() {
        return iterator((node*)0, this);
    }
    const_iterator end() const {
        return const_iterator((node*)0, this);
    }
    size_type bucket_count() const {
        return buckets.size();
    }
    size_type max_bucket_count() const {
        return __stl_prime_list[__stl_num_primes - 1];
    }
    size_type elem_in_bucket(const size_type n) const {
        size_type result = 0;
        node *cur = buckets[n];
        while(cur) {
            ++result;
            cur = cur->next;
        }
        return result;
    }
    hh_std::pair<iterator, bool> insert_unique(const value_type &x) {
        resize(num_elements + 1);
        return insert_unique_noresize(x);
    }
    hh_std::pair<iterator, bool> insert_unique_noresize(const value_type &x) {
        size_type pos = bkt_num(x);
        node *cur = buckets[pos];
        while(cur) {
            if(equals(kov(cur->data), kov(x))) {
                return hh_std::make_pair(iterator(cur, this), false);
            }
            cur = cur->next;
        }
        node *new_node = create_node(x);
        new_node->next = buckets[pos];
        buckets[pos] = new_node;
        ++num_elements;
        return hh_std::make_pair(iterator(new_node, this), true);
    }
    iterator insert_equal(const value_type &x) {
        resize(num_elements + 1);
        return insert_equal_noresize(x);
    }
    iterator insert_equal_noresize(const value_type &x) {
        size_type pos = bkt_num(x);
        node *cur = buckets[pos];
        node *new_node = create_node(x);
        while(cur) {
            if(equals(kov(cur->data), kov(x))) {
                new_node->next = cur->next;
                cur->next = new_node;
                ++num_elements;
                return iterator(new_node, this);
            }
            cur = cur->next;
        }
        new_node->next = buckets[pos];
        buckets[pos] = new_node;
        ++num_elements;
        return iterator(new_node, this);
    }
    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last) {
        insert_unique(first, last, category_type(first));
    }
    template <class InputIterator>
    void insert_equal(InputIterator first, InputIterator last) {
        insert_equal(first, last, category_type(first));
    }
    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last, input_iterator_tag) {
        while(first != last) {
            insert_unique(*first);
            ++first;
        }
    }
    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last, forward_iterator_tag) {
        size_type n = distance(first, last);
        resize(num_elements + n);
        while(first != last) {
            insert_unique_noresize(*first);
            ++first;
        }
    }
    template <class InputIterator>
    void insert_equal(InputIterator first, InputIterator last, input_iterator_tag) {
        while(first != last) {
            insert_equal(*first);
            ++first;
        }
    }
    template <class InputIterator>
    void insert_equal(InputIterator first, InputIterator last, forward_iterator_tag) {
        size_type n = distance(first, last);
        resize(num_elements + n);
        while(first != last) {
            insert_equal_noresize(*first);
            ++first;
        }
    }
    void clear() {
        node *tmp, *next;
        for(size_type i = 0; i < buckets.size(); ++i) {
            tmp = buckets[i];
            while(tmp) {
                next = tmp->next;
                destroy_node(tmp);
                tmp = next;
            }
            buckets[i] = 0;
        }
        num_elements = 0;
    }
    iterator find(const key_type &k) {
        size_type pos = bkt_num_key(k);
        node *cur = buckets[pos];
        while(cur) {
            if(equals(kov(cur->data), k))
                return iterator(cur, this);
            cur = cur->next;
        }
        return iterator(cur, this);
    }
    const_iterator find(const key_type &k) const {
        size_type pos = bkt_num_key(k);
        node *cur = buckets[pos];
        while(cur) {
            if(equals(kov(cur->data), k))
                return iterator(cur, this);
        }
        return iterator(cur, this);
    }
    size_type count(const key_type &k) const {
        size_type pos = bkt_num_key(k);
        node *cur = buckets[pos];
        size_type result = 0;
        while(cur) {
            if(equals(kov(cur->data), k))
                ++result;
            cur = cur->next;
        }
        return result;
    }
    reference find_or_insert(const value_type &x) {
        size_type pos = bkt_num(x);
        node *cur = buckets[pos];
        while(cur) {
            if(equals(kov(cur->data), kov(x)))
                return cur->data;
        }
        node *new_node = create_node(x);
        new_node->next = buckets[pos];
        buckets[pos] = new_node;
        ++num_elements;
        return new_node->data;
    }
    hh_std::pair<iterator, iterator> equal_range(const key_type &k) {
        size_type pos = bkt_num(k);
        node *cur = buckets[pos];
        while(cur) {
            if(equals(kov(cur->data), k)) {
                node *next = cur->next;
                while(next && equals(kov(next->data), k)) next = next->next;
                if(next)
                    return hh_std::make_pair(iterator(cur, this), iterator(next, this));
                for(size_type n = pos + 1; n < buckets.size(); ++n) {
                    if(buckets[n])
                        return hh_std::make_pair(iterator(cur, this), iterator(buckets[n], this));
                }
                return hh_std::make_pair(iterator(cur, this), end());
            }
            cur = cur->next;
        }
        return hh_std::make_pair(iterator((node*)0, this), iterator((node*)0, this));
    }
    hh_std::pair<const_iterator, const_iterator> equal_range(const key_type &k) const {
        size_type pos = bkt_num(k);
        node *cur = buckets[pos];
        while(cur) {
            if(equals(kov(cur->data), k)) {
                node *next = cur->next;
                while(next && equals(kov(next->data), k)) next = next->next;
                if(next)
                    return hh_std::make_pair(const_iterator(cur, this), const_iterator(next, this));
                for(size_type n = pos + 1; n < buckets.size(); ++n) {
                    if(buckets[n])
                        return hh_std::make_pair(const_iterator(cur, this), const_iterator(buckets[n], this));
                }
                return hh_std::make_pair(const_iterator(cur, this), end());
            }
            cur = cur->next;
        }
        return hh_std::make_pair(const_iterator((node*)0, this), const_iterator((node*)0, this));
    }
    size_type erase(const key_type &k) {
        size_type pos = bkt_num_key(k);
        node *tmp;
        size_type result = 0;
        while(buckets[pos] && equals(kov(buckets[pos]->data), k)) {
            tmp = buckets[pos];
            buckets[pos] = buckets[pos]->next;
            destroy_node(tmp);
            ++result;
        }
        if(buckets[pos]) {
            node *pre = buckets[pos];
            while(pre->next) {
                if(equals(kov(pre->next->data), k)) {
                    tmp = pre->next;
                    pre->next = pre->next->next;
                    destroy_node(tmp);
                } else {
                    pre = pre->next;
                }
            }
        }
        num_elements -= result;
        return result;
    }
    void erase(const iterator &it) {
        node *target = it.cur;
        if(target) {
            size_type pos = bkt_num(*it);
            if(buckets[pos] == target) {
                buckets[pos] = buckets[pos]->next;
                destroy_node(target);
                --num_elements;
            } else {
                node *cur = buckets[pos];
                while(cur->next) {
                    if(cur->next == target) {
                        cur->next = cur->next->next;
                        destroy_node(target);
                        --num_elements;
                        break;
                    }
                    cur = cur->next;
                }
            }
        }
    }
    void erase(const iterator &first, const iterator &last) {
        if(first != last) {
            if(first == begin() && last == end()) {
                clear();
                return;
            }
            size_type pos = bkt_num(*first);
            node *start = first.cur, *finish = last.cur, *node, *tmp;
            if(last == end()) {
                if(buckets[pos] == start) {
                    while(buckets[pos]) {
                        tmp = buckets[pos];
                        buckets[pos] = tmp->next;
                        destroy_node(tmp);
                        --num_elements;
                    }
                } else {
                    tmp = buckets[pos];
                    while(tmp->next && tmp->next != start) tmp = tmp->next;
                    while(tmp->next) {
                        node = tmp->next;
                        tmp->next = tmp->next->next;
                        destroy_node(node);
                        --num_elements;
                    }
                }
                ++pos;
                for(; pos < buckets.size(); ++pos) {
                    if(buckets[pos]) {
                        tmp = buckets[pos];
                        buckets[pos] = tmp->next;
                        destroy_node(tmp);
                        --num_elements;
                    }
                }
            } else {
                if(buckets[pos] == start) {
                    while(buckets[pos] && buckets[pos] != finish) {
                        tmp = buckets[pos];
                        buckets[pos] = tmp->next;
                        destroy_node(tmp);
                        --num_elements;
                    }
                    if(!buckets[pos]) {
                        ++pos;
                        for(; pos < buckets.size(); ++pos) {
                            if(!buckets[pos]) continue;
                            while(buckets[pos] && buckets[pos] != finish) {
                                tmp = buckets[pos];
                                buckets[pos] = tmp->next;
                                destroy_node(tmp);
                                --num_elements;
                            }
                            if(!buckets[pos]) break;
                        }
                    }
                } else {
                    node = buckets[pos];
                    while(node->next && node->next != start) node = node->next;
                    while(node->next && node->next != finish) {
                        tmp = node->next;
                        node->next = node->next->next;;
                        destroy_node(tmp);
                        --num_elements;
                    }
                    if(!node->next) {
                        ++pos;
                        for(; pos < buckets.size(); ++pos) {
                            if(!buckets[pos]) continue;
                            while(buckets[pos] && buckets[pos] != finish) {
                                tmp = buckets[pos];
                                buckets[pos] = tmp->next;
                                destroy_node(tmp);
                                --num_elements;
                            }
                            if(!buckets[pos]) break;
                        }
                    }
                }
            }
        }
    }
    void erase(const const_iterator &it) {
        erase(iterator(const_cast<node*>(it.cur), this));
    }
    void erase(const const_iterator &first, const const_iterator &last) {
        erase(iterator(const_cast<node*>(first.cur), this), iterator(const_cast<node*>(last.cur), this));
    }
    friend bool operator== <> (const hashtable &x, const hashtable &y);

private:
    void resize(size_type n) {
        if(n > buckets.size()) {
            size_type new_size = __stl_next_prime(n);
            if(new_size >= n) {
                vector<node*, Alloc> tmp(new_size, (node*)0);
                for(size_type i = 0; i < buckets.size(); ++i) {
                    node *first = buckets[i];
                    while(first) {
                        buckets[i] = first->next;
                        size_type pos = bkt_num(first->data, new_size);
                        first->next = tmp[pos];
                        tmp[pos] = first;
                        first = buckets[i];
                    }
                }
                buckets.swap(tmp);
            }
        }
    }
    size_type bkt_num(const value_type &x) const {
        return bkt_num_key(kov(x));
    }
    size_type bkt_num(const value_type &x, size_type n) const {
        return bkt_num_key(kov(x), n);
    }
    size_type bkt_num_key(const key_type&x) const {
        return bkt_num_key(x, buckets.size());
    }
    size_type bkt_num_key(const key_type&x, size_type n) const {
        return hash(x) % n;
    }
    node* create_node(const value_type &x) {
        node* new_node;
        __STL_TRY {
            new_node = node_allocater::allocate();
            construct(&(new_node->data), x);
            new_node->next = 0;
        }
        __STL_UNWIND(destroy_node(new_node))
        return new_node;
    }
    void destroy_node(node* n) {
        destory(&(n->data));
        node_allocater::deallocate(n);
    }
    void initialize_buckets(size_type n) {
        n = __stl_next_prime(n);
        buckets.reserve(n);
        buckets.insert(buckets.end(), n, (node*)0);
        num_elements = 0;
    }
    void copy_from(const hashtable &h) {
        buckets.reserve(h.buckets.size());
        buckets.insert(buckets.end(), buckets.size(), (node*)0);
        node *new_node;
        for(size_type i = 0; i < h.buckets.size(); ++i) {
            if(const node* cur = h.buckets[i]) {
                new_node = create_node(cur->data);
                buckets[i] = new_node;
                cur = cur->next;
                while(cur) {
                    new_node->next = create_node(cur->data);
                    new_node = new_node->next;
                }
            }
        }
        num_elements = h.num_elements;
    }
};

};

#endif
