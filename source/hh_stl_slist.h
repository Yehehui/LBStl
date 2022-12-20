#ifndef __HH_STL_SLIST_H_
#define __HH_STL_SLIST_H_

#include<cstddef>
#include "hh_stl_iterator.h"
#include "hh_stl_alloc.h"
#include "hh_config.h"
#include "hh_stl_construct.h"

namespace hh_std {

struct __slist_node_base {
    __slist_node_base *next;
};

inline __slist_node_base *__slist_make_link(__slist_node_base *prev_node, __slist_node_base *new_node) {
    new_node->next = prev_node->next;
    prev_node->next = new_node;
    return new_node;
}

inline size_t __slist_size(const __slist_node_base *node) {
    size_t result = 0;
    while(node) {
        node = node->next;
        ++result;
    }
    return result;
}

inline __slist_node_base *__slist_previous(__slist_node_base *head, __slist_node_base *node) {
    while(head && head->next != node) {
        head = head->next;
    }
    return head;
}

inline const __slist_node_base *__slist_previous(const __slist_node_base *head, const __slist_node_base *node) {
    while(head && head->next != node) {
        head = head->next;
    }
    return head;
}

inline void __slist_splice_after(__slist_node_base *pos, __slist_node_base *before_first, __slist_node_base *before_last) {
    if (pos != before_first->next && pos != before_last) {
        __slist_node_base *after = pos->next;
        pos->next = before_first->next;
        before_first->next = before_last->next;
        before_last->next = after;
    }
}

inline __slist_node_base *__slist_reverse(__slist_node_base *node) {
    if(!node && !node->next) return node;
    __slist_node_base *result = node, *next_node;
    node = node->next;
    while(node) {
        next_node = node->next;
        node->next = result;
        result = node;
        node = next_node;
    }
    return result;
}

template <class T>
struct __slist_node: public __slist_node_base {
    T data;
};

struct __slist_iterator_base {
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef forward_iterator_tag iterator_category;

    __slist_node_base *node;
    __slist_iterator_base(__slist_node_base *n): node(n) {}

    void incr () {
        node = node->next;
    }

    bool operator== (const __slist_iterator_base &x) {
        return node == x.node;
    }
    bool operator!= (const __slist_iterator_base &x) {
        return node != x.node;
    }
};

template <class T, class Ref, class Ptr>
struct __slist_iterator: public __slist_iterator_base {
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __slist_iterator<T, T&, T*> iterator;
    typedef __slist_iterator<T, const T&, const T*> const_iterator;
    typedef __slist_iterator<T, Ref, Ptr> self;
    typedef __slist_node<T> list_node;

    __slist_iterator(list_node *x): __slist_iterator_base(x) {}
    __slist_iterator(): __slist_iterator_base(0) {}
    __slist_iterator(const_iterator &x): __slist_iterator_base(x.node) {}

    reference operator* () const {
        return ((list_node*)node)->data;
    }

    pointer operator-> () const {
        return &(operator*());
    }

    self &operator++ () {
        incr();
        return *this;
    }

    self operator++ (int) {
        __slist_iterator tmp = *this;
        tmp.incr();
        return tmp;
    }
};

template <class T, class Alloc>
class slist;

template <class T, class Alloc>
bool operator== (const slist<T, Alloc> &l, const slist<T, Alloc> &l2) {
    __slist_node<T> *cur = (__slist_node<T>*)l.head.next, *cur_l = (__slist_node<T>*)l2.head.next;
    while(cur && cur_l) {
        if(cur->data != cur_l->data)
            return false;
        cur = (__slist_node<T>*)cur->next;
        cur_l = (__slist_node<T>*)cur_l->next;
    }
    return cur == cur_l;
}

template <class T, class Alloc = alloc>
class slist {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __slist_iterator<T, T&, T*> iterator;
    typedef __slist_iterator<T, const T&, const T*> const_iterator;
private:
    typedef __slist_node_base list_node_base;
    typedef __slist_node<T> list_node;
    typedef __slist_iterator_base iterator_base;
    typedef hh_std::simple_alloc<list_node, Alloc> data_allocate;

    list_node_base head;

    static list_node * create_node(const value_type & x) {
        list_node *result;
        __STL_TRY {
            result = data_allocate::allocate();
            construct(&(result->data), x);
            result->next = 0;
        }
        __STL_UNWIND(data_allocate::deallocate(result));
        return result;
    }

    static void destroy_node(list_node * x) {
        destory(&(x->data));
        data_allocate::deallocate(x);
    }

    void fill_initialize(size_type n, const value_type & x) {
        head.next = 0;
        __STL_TRY {
            __insert_after_fill(&head, n, x);
        }
        __STL_UNWIND(clear());
    }

    void __insert_after_fill(list_node_base * node, size_type n, const value_type & x) {
        for(size_type sz = 0; sz < n; ++sz) {
            __slist_make_link(node, create_node(x));
        }
    }

    template <class InputIterator>
    void range_initialize(InputIterator first, InputIterator last) {
        head.next = 0;
        __STL_TRY {
            __insert_after_range(&head, first, last);
        }
        __STL_UNWIND(clear());
    }

    template <class InputIterator>
    void __insert_after_range(list_node_base * node, InputIterator first, InputIterator last) {
        while(first != last) {
            node = __slist_make_link(node, create_node(*first));
            ++first;
        }
    }

    list_node * __insert_after(__slist_node_base * node, const value_type & x) {
        return (list_node*)__slist_make_link(node, create_node(x));
    }

    list_node_base * erase_after(__slist_node_base * node) {
        list_node *tmp = (list_node*)node->next;
        node->next = node->next->next;
        destroy_node(tmp);
        return node->next;
    }
    list_node_base * erase_after(__slist_node_base * before_first, __slist_node_base * last) {
        list_node *tmp = (list_node*)before_first->next;
        while(tmp != last) {
            before_first->next = before_first->next->next;
            destroy_node(tmp);
            tmp = (list_node*) before_first->next;
        }
        return last;
    }


public:
    slist() {
        head.next = 0;
    }
    slist(size_type n, const value_type & x) {
        fill_initialize(n, x);
    }
    slist(int n, const value_type & x) {
        fill_initialize(n, x);
    }
    slist(long n, const value_type & x) {
        fill_initialize(n, x);
    }
    explicit slist(size_type n) {
        fill_initialize(n, value_type());
    }
    template <class InputIterator>
    slist(InputIterator first, InputIterator last) {
        range_initialize(first, last);
    }
    slist(const slist & l) {
        range_initialize(l.begin(), l.end());
    }
    slist &operator= (const slist & l) {
        if(*this != l) {
            iterator first = begin(), first_l = l.begin();
            iterator last = end(), last_l = l.end();
            __slist_node_base *node;
            while(first != last && first_l != last_l) {
                destory(&*first);
                construct(&*first, *first_l);
            }
            if(first != last) {
                node = __slist_previous(&head, &*first);
                __slist_node_base *tmp;
                while(node->next) {
                    tmp = node->next;
                    node->next = node->next->next;
                    destroy_node(tmp);
                }
            }
            if(first_l != last_l) {
                node = __slist_previous(&head, &*first);
                while(first_l != last_l) {
                    node = __slist_make_link(node, create_node(*first_l));
                    ++first_l;
                }
            }
            return *this;
        }
    }
    ~slist() {
        clear();
    }
    iterator begin() {
        return iterator((list_node*)head.next);
    }
    const_iterator begin() const {
        return const_iterator((list_node*)head.next);
    }
    iterator end() {
        return iterator(0);
    }
    const_iterator end() const {
        return const_iterator(0);
    }
    size_type size() const {
        return __slist_size(head.next);
    }
    size_type max_size() const {
        return size_type(-1);
    }
    bool empty() const {
        return head.next == 0;
    }
    void swap(slist & l) {
        std::swap(head, l.head);
    }
    void clear() {
        list_node *tmp;
        while(head.next) {
            tmp = (list_node*)head.next;
            head.next = head.next->next;
            destroy_node(tmp);
        }
    }
    reference front() {
        return ((list_node*)head.next)->data;
    }
    const_reference front() const {
        return ((list_node*)head.next)->data;
    }
    void push_front(const value_type & x) {
        __slist_make_link(&head, create_node(x));
    }
    void pop_front() {
        list_node *node = (list_node*)head.next;
        head.next = (head.next)->next;
        destroy_node(node);
    }
    iterator previous(const_iterator pos) {
        return iterator((list_node*)__slist_previous(&head, pos.node));
    }
    const_iterator previous(const_iterator pos) const {
        return const_iterator((list_node*)__slist_previous(&head, pos.node));
    }
    iterator insert_after(iterator pos, const value_type & x) {
        return iterator(__insert_after(pos.node, x));
    }
    iterator insert_after(iterator pos) {
        return iterator(__insert_after(pos.node, value_type()));
    }
    void insert_after(iterator pos, size_type n, const value_type & x) {
        __insert_after_fill(pos.node, n, x);
    }
    void insert_after(iterator pos, int n, const value_type & x) {
        __insert_after_fill(pos.node, size_type(n), x);
    }
    void insert_after(iterator pos, long n, const value_type & x) {
        __insert_after_fill(pos.node, size_type(n), x);
    }
    template <class InputIterator>
    void insert_after(iterator pos, InputIterator first, InputIterator last) {
        __insert_after_range(pos.node, first, last);
    }
    iterator insert(iterator pos, const value_type & x) {
        return iterator(__insert_after(__slist_previous(&head, pos.node), x));
    }
    iterator insert(iterator pos) {
        return iterator(__insert_after(__slist_previous(&head, pos.node), value_type()));
    }
    iterator erase_after(iterator pos) {
        return iterator((list_node*)erase_after(pos.node));
    }
    iterator erase_after(iterator before_first, iterator last) {
        return iterator((list_node*)erase_after(before_first.node, last.node));
    }
    iterator erase(iterator pos) {
        return iterator((list_node*)erase_after(__slist_previous(&head, pos.node)));
    }
    iterator erase(iterator first, iterator last) {
        return iterator((list_node*)erase_after(__slist_previous(&head, first.node), last.node));
    }
    void resize(size_type new_size, const value_type & x) {
        list_node_base *tmp = &head;
        size_type sz = 0;
        while(tmp->next && sz < new_size) {
            tmp = tmp->next;
            ++sz;
        }
        if(tmp->next) {
            erase_after(tmp, 0);
        }
        if(sz < new_size) {
            __insert_after_fill(tmp, new_size - sz, x);
        }
    }
    void resize(size_type new_size) {
        resize(new_size, value_type());
    }
    void splice_after(iterator pos, iterator before_first, iterator before_last) {
        __slist_splice_after(pos.node, before_first.node, before_last.node);
    }
    void splice_after(iterator pos, iterator prev) {
        __slist_splice_after(pos.node, prev.node, (prev.node)->next);
    }
    void splice_after(iterator pos, slist & l) {
        if(l.head.next) {
            __slist_splice_after(pos.node, l.head, __slist_previous(&(l.head), 0));
        }
    }
    void splice_after(iterator pos, slist & l, iterator i) {
        __slist_splice_after(__slist_previous(&head, pos.node), __slist_previous(&(l.head), i.node), i.node);
    }
    void splice_after(iterator pos, slist & l, iterator first, iterator last) {
        __slist_splice_after(__slist_previous(&head, pos.node), __slist_previous(&(l.head), first.node), __slist_previous(&(l.head), last.node));
    }
    void reverse() {
        if(head.next) {
            head.next = __slist_reverse(head.next);
        }
    }
    template <class Predicate>
    void remove_if(Predicate predic) {
        list_node_base *cur = &head;
        while(cur->next) {
            if(predic(((list_node*)cur->next)->data)) {
                erase_after(cur);
            } else {
                cur = cur->next;
            }
        }
    }
    void remove(const value_type & x) {
        remove_if([x](const value_type & val) {
            return x == val;
        });
    }
    template <class BinaryPredicate>
    void unique(BinaryPredicate predic) {
        list_node_base *cur = head.next;
        while(cur && cur->next) {
            if(predic(((list_node*)cur)->data, ((list_node*)cur->next)->data)) {
                erase_after(cur);
            } else {
                cur = cur->next;
            }
        }
    }
    void unique() {
        unique([](const value_type & x, const value_type & y) {
            return x == y;
        });
    }
    template <class StrictWeakOrdering>
    void merge(slist & l, StrictWeakOrdering order) {
        list_node_base *cur = &head, *cur_l = &(l.head);
        list_node_base *tmp;
        while(cur->next && cur_l->next) {
            if(order(((list_node*)cur->next)->data, ((list_node*)cur_l->next)->data)) {
                cur = cur->next;
            } else {
                tmp = cur_l->next;
                cur_l->next = cur_l->next->next;
                __slist_make_link(cur, tmp);
                cur = cur->next;
            }
        }
        while(cur_l->next) {
            cur->next = cur_l->next;
            cur_l->next = 0;
        }
    }
    void merge(slist & l) {
        merge(l, [](const value_type & x, const value_type & y) {
            return x < y;
        });
    }
    template <class StrictWeakOrdering>
    void sort(StrictWeakOrdering order) {
        if(head.next == 0 || head.next->next == 0) return;
        slist<T, Alloc> avail;
        slist<T, Alloc> counts[64];
        size_type fill = 0;
        list_node_base *tmp;
        while(!empty()) {
            tmp = head.next;
            head.next = head.next->next;
            __slist_make_link(&(avail.head), tmp);
            size_type i = 0;
            while(i < fill && !counts[i].empty()) {
                avail.merge(counts[i++], order);
            }
            counts[i].swap(avail);
            if(i == fill) fill++;
        }
        for(size_type i = 0; i < fill; ++i) {
            merge(counts[i]);
        }
    }
    void sort() {
        sort([](const value_type & x, const value_type & y) {
            return x < y;
        });
    }
    void display() {
        list_node *cur = (list_node*)head.next;
        while(cur) {
            std::cout << cur->data << " ";
            cur = (list_node*) cur->next;
        }
        std::cout << std::endl;
    }

public:
    friend bool operator== <> (const slist<T, Alloc> &l1, const slist<T, Alloc> &l2);
};

template <class T, class Alloc>
inline void swap(const slist<T, Alloc> &l, const slist<T, Alloc> &l2) {
    l.swap(l2);
}

template <class T, class Alloc>
inline bool operator< (const slist<T, Alloc> &l, const slist<T, Alloc> &l2) {
    typedef typename slist<T, Alloc>::const_iterator iterator;
    iterator first = l.begin(), first_l = l2.begin();
    iterator last = l.end(), last_l = l2.end();
    while(first != last && first_l != last_l) {
        if(*first < *first_l) {
            return true;
        }
        if(*first > *first_l) {
            return false;
        }
        ++first;
        ++first_l;
    }
    return first_l != last_l;
}

};

#endif
