#ifndef __HH_STL_LIST_H_
#define __HH_STL_LIST_H_

#include<cstddef>
#include "hh_stl_iterator.h"
#include "hh_stl_alloc.h"
#include "hh_stl_construct.h"
#include "hh_config.h"

namespace hh_std {

template <class T>
struct __list_node {
    typedef __list_node<T>* pointer;
    pointer prev;
    pointer next;
    T data;
};

template <class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, const T&, const T*> const_iterator;
    typedef __list_iterator<T, Ref, Ptr> self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __list_node<T>* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    link_type node;

    __list_iterator() = default;
    __list_iterator(link_type x): node(x) {}
    __list_iterator(const iterator &it): node(it.node) {}

    bool operator== (const __list_iterator &it) const {
        return node == it.node;
    }

    bool operator!= (const __list_iterator &it) const {
        return node != it.node;
    }

    reference operator* () const {
        return (*node).data;
    }

    Ptr operator-> () const {
        return &(operator*());
    }

    self &operator++ () {
        node = node->next;
        return *this;
    }

    self operator++ (int) {
        self tmp = *this;
        node = node->next;
        return tmp;
    }

    self &operator-- () {
        node = node->prev;
        return *this;
    }

    self operator-- (int) {
        self tmp = *this;
        node = node->prev;
        return tmp;
    }
};

template <class T, class Alloc = alloc>
class list {
protected:
    typedef __list_node<T> list_node;
    typedef simple_alloc<__list_node<T>, Alloc> data_allocater;
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef list_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, const T&, const T*> const_iterator;

    typedef reverse_iterator<iterator> r_iterator;
    typedef reverse_iterator<const_iterator> const_r_iterator;

protected:
    link_type node;
    link_type get_node() {
        return (link_type)data_allocater::allocate();
    }

    void put_node(link_type p) {
        data_allocater::deallocate(p, size_type(1));
    }

    link_type create_node(const T &x) {
        link_type node = get_node();
        construct(&node->data, x);
        return node;
    }

    void destory_node(link_type p) {
        destory(p);
        put_node(p);
    }

    void empty_initialize() {
        node = get_node();
        node->next = node;
        node->prev = node;
    }


public:
    list() {
        empty_initialize();
    }

    iterator begin() {
        return iterator(node->next);
    }

    const_iterator begin() const {
        return const_iterator(node->next);
    }

    iterator end() {
        return iterator(node);
    }

    const_iterator end() const {
        return const_iterator(node);
    }

    r_iterator rbegin() {
        return r_iterator(end());
    }

    const_r_iterator rbegin() const {
        return const_r_iterator(end());
    }

    r_iterator rend() {
        return r_iterator(begin());
    }

    const_r_iterator rend() const {
        return const_r_iterator(begin());
    }

    bool empty() const {
        return node->next == node;
    }

    size_type size() const {
        size_type n = distance(begin(), end());
        return n;
    }

    size_type max_size() const {
        return size_type(-1);
    }

    reference front() {
        return *begin();
    }

    const_reference front() const {
        return *begin();
    }

    reference back() {
        return *(--end());
    }

    const_reference back() const {
        return *(--end());
    }

    void swap(list<T, Alloc> &x) {
        std::swap(node, x.node);
    }

    void fill_initialize(size_type n, const T &x) {
        empty_initialize();
        insert(begin(), n, x);
    }

    template <class InputIterator>
    void range_initialize(InputIterator first, InputIterator last) {
        empty_initialize();
        insert(begin(), first, last);
    }

    iterator insert(iterator pos, const T &x) {
        link_type tmp = create_node(x);
        tmp->next = pos.node;
        tmp->prev = (pos.node)->prev;
        tmp->prev->next = tmp;
        tmp->next->prev = tmp;
        return iterator(tmp);
    }

    iterator insert(iterator pos) {
        return insert(pos, T());
    }

    template <class InputIterator>
    iterator insert(iterator pos, InputIterator first, InputIterator last);

    void insert(iterator pos, size_type n, const T &x);

    void insert(iterator pos, int n, const T &x) {
        insert(pos, size_type(n), x);
    }

    void insert(iterator pos, long n, const T &x) {
        insert(pos, size_type(n), x);
    }

    void push_back(const T &x) {
        insert(end(), x);
    }

    void push_front(const T &x) {
        insert(begin(), x);
    }

    iterator erase(iterator pos) {
        link_type tmp = pos.node;
        tmp->prev->next = tmp->next;
        tmp->next->prev = tmp->prev;
        link_type result = tmp->next;
        put_node(tmp);
        return iterator(result);
    }

    iterator erase(iterator first, iterator last);
    void resize(size_type new_size, const T &x);
    void resize(size_type new_size) {
        resize(new_size, T());
    }

    void clear();

    void pop_front() {
        erase(begin());
    }

    void pop_back() {
        erase(--end());
    }

    list(size_type n, const T &x) {
        fill_initialize(n, x);
    }
    list(int n, const T &x) {
        fill_initialize(n, x);
    }
    list(long n, const T &x) {
        fill_initialize(n, x);
    }
    explicit list(size_type n) {
        fill_initialize(n, T());
    }
    template <class InputIterator>
    list(InputIterator first, InputIterator last) {
        range_initialize(first, last);
    }

    list(const list<T, Alloc> &x) {
        range_initialize(x.begin(), x.end());
    }

    ~list() {
        clear();
        put_node(node);
    }

    list<T, Alloc> &operator= (const list<T, Alloc> &x);

    void play_all() {
        iterator cur = begin();
        while(cur != end()) {
            std::cout << *cur << " ";
            ++cur;
        }
        std::cout << std::endl;
    }

protected:
    void transfer(iterator pos, iterator first, iterator last) {
        if(pos != last && pos != first) {
            iterator tmp = last;
            tmp--;
            first.node->prev->next = last.node;
            last.node->prev = first.node->prev;
            first.node->prev = pos.node->prev;
            tmp.node->next = pos.node;
            first.node->prev->next = first.node;
            tmp.node->next->prev = tmp.node;
        }
    }

public:
    void splice(iterator pos, list<T, Alloc> &x) {
        if(x != *this && !x.empty()) {
            transfer(pos, x.begin(), x.end());
        }
    }

    void splice(iterator pos, list<T, Alloc>&, iterator x) {
        iterator tmp = x;
        tmp++;
        transfer(pos, x, tmp);
    }

    void splice(iterator pos, list<T, Alloc>&, iterator first, iterator last) {
        transfer(pos, first, last);
    }

    void remove(const T &x);
    void unique();
    void merge(list<T, Alloc> &x);
    void reverse();
    void sort();

    template <class Predicate> void remove_if(Predicate);
    template <class BinaryPredicate> void unique(BinaryPredicate);
    template <class StrictWeakOrdering> void merge(list&, StrictWeakOrdering);
    template <class StrictWeakOrdering> void sort(StrictWeakOrdering);

    template <class T1, class Alloc1>
    friend bool operator== (const list<T1, Alloc1> &x, const list<T1, Alloc1> &y);
};

template <class T, class Alloc>
inline bool operator== (const list<T, Alloc> &x, const list<T, Alloc> &x1) {
    typedef typename list<T, Alloc>::iterator iterator;
    iterator first_x = x.begin(), last_x = x.end();
    iterator first_x1 = x1.begin(), last_x1 = x1.end();
    while(first_x != last_x && first_x1 != last_x1) {
        if(*first_x != *first_x1) {
            return false;
        }
        ++first_x;
        ++first_x1;
    }
    if(first_x != last_x || first_x1 != last_x1) {
        return false;
    }
    return true;
}

template <class T, class Alloc>
inline bool operator<(const list<T, Alloc>& x, const list<T, Alloc>& x1) {
    typedef typename list<T, Alloc>::iterator iterator;
    iterator first_x = x.begin(), last_x = x.end();
    iterator first_x1 = x1.begin(), last_x1 = x1.end();
    while(first_x != last_x && first_x1 != last_x1) {
        if(*first_x < *first_x1) {
            return true;
        }
        if(*first_x > *first_x1) {
            return false;
        }
        ++first_x;
        ++first_x1;
    }
    return first_x1 != last_x1;
}

template <class T, class Alloc>
inline void swap(list<T, Alloc>& x, list<T, Alloc>& y) {
    x.swap(y);
}

template <class T, class Alloc>
inline list<T, Alloc> &list<T, Alloc>::operator= (const list<T, Alloc> &x) {
    if(*this != x) {
        iterator first = begin(), last = end();
        iterator first_x = x.begin(), last_x = x.end();
        while(first != last && first_x != last_x) {
            *first = *first_x;
            ++first;
            ++first_x;
        }
        while(first != last) {
            first = erase(first);
        }
        if(first_x != last_x) {
            insert(last, first_x, last_x);
        }
        return *this;
    }
}

template <class T, class Alloc>
template <class InputIterator>
inline typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator pos, InputIterator first, InputIterator last) {
    while(first != last) {
        pos = insert(pos, *(--last));
    }
    return pos;
}

template <class T, class Alloc>
inline void list<T, Alloc>::insert(iterator pos, size_type n, const T &x) {
    while(n > 0) {
        pos = insert(pos, x);
        n--;
    }
}

template <class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last) {
    iterator result;
    while(first != last) {
        result = erase(first++);
    }
    return result;
}

template <class T, class Alloc>
inline void list<T, Alloc>::resize(size_type new_size, const T &x) {
    size_type old_size = size();
    if(old_size < new_size) {
        while(old_size < new_size) {
            insert(end(), x);
            old_size++;
        }
    } else if(old_size > new_size) {
        while(old_size > new_size) {
            erase(--end());
            old_size--;
        }
    }
}

template <class T, class Alloc>
inline void list<T, Alloc>::clear() {
    while(node->next != node) {
        erase(begin());
    }
}

template <class T, class Alloc>
template <class Predicate>
inline void list<T, Alloc>::remove_if(Predicate predic) {
    iterator tmp = begin();
    while(tmp != end()) {
        if(predic(*tmp)) {
            tmp = erase(tmp);
        } else {
            ++tmp;
        }
    }
}

template <class T, class Alloc>
inline void list<T, Alloc>::remove(const T &x) {
    remove_if([x](const T & t) {
        return t == x;
    });
}

template <class T, class Alloc>
template <class BinaryPredicate>
inline void list<T, Alloc>::unique(BinaryPredicate predic) {
    if(size() > 1) {
        iterator prev = begin(), next = prev;
        ++next;
        while(next != end()) {
            if(predic(*prev, *next)) {
                next = erase(next);
            } else {
                prev = next;
                ++next;
            }
        }
    }
}

template <class T, class Alloc>
inline void list<T, Alloc>::unique() {
    unique([](const T & a, const T & b) {
        return a == b;
    });
}

template <class T, class Alloc>
template <class StrictWeakOrdering>
inline void list<T, Alloc>::merge(list<T, Alloc> &x, StrictWeakOrdering order) {
    iterator first = begin(), last = end();
    iterator first_x = x.begin(), last_x = x.end();
    iterator tmp;
    while(first != last && first_x != last_x) {
        if(order(*first, *first_x)) {
            tmp = first_x;
            ++tmp;
            transfer(first, first_x, tmp);
            first_x = tmp;
        } else {
            ++first;
        }
    }
    if(first_x != last_x) {
        transfer(end(), first_x, last_x);
    }
}

template <class T, class Alloc>
inline void list<T, Alloc>::merge(list<T, Alloc> &x) {
    merge(x, [](const T & a, const T & b) {
        return a > b;
    });
}

template <class T, class Alloc>
inline void list<T, Alloc>::reverse() {
    if(size() > 1) {
        iterator prev = begin();
        ++prev;
        iterator next = prev;
        ++next;
        while(prev != end()) {
            transfer(begin(), prev, next);
            prev = next;
            next++;
        }
    }
}

template <class T, class Alloc>
template <class StrictWeakOrdering>
inline void list<T, Alloc>::sort(StrictWeakOrdering order) {
    if(size() > 1) {
        list<T, Alloc> avail, counts[64];
        size_type fill = 0;
        while(!empty()) {
            avail.splice(avail.begin(), *this, begin());
            size_type i = 0;
            while(i < fill && !counts[i].empty()) {
                counts[i].merge(avail, order);
                avail.swap(counts[i]);
                i++;
            }
            avail.swap(counts[i]);
            if(i == fill) fill++;
        }
        for(size_type i = 0; i < fill; i++) {
            merge(counts[i], order);
        }
    }
}

template <class T, class Alloc>
inline void list<T, Alloc>::sort() {
    sort([](const T & a, const T & b) {
        return a > b;
    });
}

}
#endif
