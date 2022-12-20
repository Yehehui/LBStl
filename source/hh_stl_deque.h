#ifndef __HH_STL_DEQUE_H_
#define __HH_STL_DEQUE_H_

#include<cstddef>
#include "hh_stl_iterator.h"
#include "hh_stl_alloc.h"
#include "hh_stl_uninitialized.h"
#include "hh_stl_construct.h"

namespace hh_std {

inline size_t deque_buf_size(size_t buf_size, size_t sz) {
    return buf_size == 0 ? (sz > 512 ? 1 : 512 / sz) : buf_size;
}

template <class T, class Ref, class Ptr, size_t BufSize>
struct __deque_iterator {
    typedef __deque_iterator<T, T&, T*, BufSize> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSize> const_iterator;
    typedef __deque_iterator self;

    static size_t buffer_size() {
        return deque_buf_size(BufSize, sizeof(T));
    }

    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
    typedef T** map_pointer;

    T* cur;
    T* first;
    T* last;
    map_pointer node;

    __deque_iterator(const T* x, map_pointer y): cur(x), first(x), last(x + buffer_size()), node(y) {}
    __deque_iterator() = default;
    __deque_iterator(const __deque_iterator &x): cur(x.cur), first(x.first), last(x.last), node(x.node) {}

    reference operator* () const {
        return *cur;
    }

    pointer operator-> () const {
        return *(operator*());
    }

    difference_type operator- (const self &x) const {
        return (node - x.node - 1) * difference_type(buffer_size()) + (x.last - x.cur) + (cur - first);
    }

    void set_node(map_pointer p) {
        node = p;
        first = *node;
        last = first + difference_type(buffer_size());
    }

    self &operator++ () {
        ++cur;
        if (cur == last) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }

    self operator++ (int) {
        self tmp = *this;
        ++(*this);
        return tmp;
    }

    self &operator-- () {
        if(cur == first) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator-- (int) {
        self tmp = *this;
        --(*this);
        return tmp;
    }

    self &operator+= (difference_type n) {
        difference_type bs = difference_type(buffer_size());
        difference_type offset = n + (cur - first);
        if(offset >= 0 && offset < bs) {
            cur += n;
        } else {
            difference_type node_offset = offset > 0 ? offset / bs : -(-offset - 1) / bs - 1;
            set_node(node + node_offset);
            cur = first + (offset - node_offset * bs);
        }
        return *this;
    }

    self &operator-= (difference_type n) {
        return operator+=(-n);
    }

    self operator+ (difference_type n) {
        self tmp = *this;
        tmp += n;
        return tmp;
    }

    self operator- (difference_type n) {
        self tmp = *this;
        tmp -= n;
        return tmp;
    }

    reference operator[] (size_type n) {
        return *(*this + n);
    }

    bool operator== (const self &x) {
        return cur == x.cur;
    }

    bool operator!= (self &x) {
        return cur != x.cur;
    }

    bool operator< (self &x) {
        return node == x.node ? cur < x.cur : (node < x.node);
    }

};

template <class T, class Alloc = alloc, size_t BufSize = 0>
class deque {
public:
    typedef T value_type;
    typedef T* pointer;
    typedef const T* const_pointer;
    typedef T& reference;
    typedef const T& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef __deque_iterator<T, T&, T*, BufSize> iterator;
    typedef __deque_iterator<T, const T&, const T*, BufSize> const_iterator;

    typedef reverse_iterator<iterator> r_iterator;
    typedef reverse_iterator<const_iterator> const_r_iterator;

protected:
    typedef pointer* map_pointer;
    typedef simple_alloc<T, Alloc> data_allocater;
    typedef simple_alloc<pointer, Alloc> map_allocater;

    static size_type buffer_size() {
        return deque_buf_size(BufSize, sizeof(value_type));
    }

    static size_type initial_map_size() {
        return 8;
    }

    iterator start;
    iterator finish;

    map_pointer map;

    size_type map_size;

public:
    iterator begin() {
        return start;
    }
    const_iterator begin() const {
        return start;
    }
    iterator end() {
        return finish;
    }
    const_iterator end() const {
        return finish;
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
    reference operator[] (size_type n) {
        return *(start + n);
    }
    const_reference operator[] (size_type n) const {
        return *(start + n);
    }
    reference front() {
        return *start;
    }
    const_reference front() const {
        return *start;
    }
    reference back() {
        return *(finish - 1);
    }
    const_reference back() const {
        return *(finish - 1);
    }
    size_type size() const {
        return finish - start;
    }
    size_type max_size() const {
        return size_type(-1);
    }
    bool empty() const {
        return start == finish;
    }
    deque(): start(), finish(), map(0), map_size(0) {
        create_map_and_nodes(0);
    }
    deque(size_type n, const value_type &x): start(), finish(), map(0), map_size(0) {
        fill_initialize(n, x);
    }
    deque(int n, const value_type &x): start(), finish(), map(0), map_size(0) {
        fill_initialize(n, x);
    }
    deque(long n, const value_type &x): start(), finish(), map(0), map_size(0) {
        fill_initialize(n, x);
    }
    deque(size_type n): start(), finish(), map(0), map_size(0) {
        fill_initialize(n, value_type());
    }
    deque(const deque &x): start(), finish(), map(0), map_size(0) {
        create_map_and_nodes(x.size());
        uninitialized_copy(x.begin(), x.end(), start);
    }
    deque(const_pointer first, const_pointer last): start(), finish(), map(0), map_size(0) {
        create_map_and_nodes(last - first);
        uninitialized_copy(first, last, start);
    }
    deque(const_iterator first, const_iterator last) : start(), finish(), map(0), map_size(0) {
        create_map_and_nodes(last - first);
        uninitialized_copy(first, last, start);
    }
    template <class InputIterator>
    deque(InputIterator first, InputIterator last): start(), finish(), map(0), map_size(0) {
        range_initialize(first, last, category_type(first));
    }
    ~deque() {
        destory(start.cur, finish.cur);
        destory_map_and_nodes();
    }
    deque &operator= (const deque &x) {
        if(*this != x) {
            if(size() == x.size()) {
                std::copy(x.begin, x.end(), start);
            } else if(size() > x.size()) {
                std::copy(x.begin(), x.end(), start);
                erase(finish - (size() - x.size()), finish);
            } else {
                std::copy(x.begin(), x.begin() + difference_type(size()), start);
                insert(finish, x.begin() + difference_type(size()), x.end());
            }
        }
        return *this;
    }
    void swap(deque &x) {
        std::swap(map, x.map);
        std::swap(map_size, x.map_size);
        std::swap(start, x.start);
        std::swap(finish, x.finish);
    }
    void push_back(const value_type &x) {
        if(finish.cur == finish.last - 1) {
            push_back_aux(x);
        } else {
            construct(finish.cur, x);
            ++finish;
        }
    }
    void push_front(const value_type &x) {
        if(start.cur == start.first) {
            push_front_aux(x);
        } else {
            --start;
            construct(start.cur, x);
        }
    }
    void pop_back() {
        if(finish.cur == finish.first) {
            pop_back_aux();
        } else {
            --finish;
            destory(finish.cur);
        }
    }
    void pop_front() {
        if(start.cur == start.last - 1) {
            pop_front_aux();
        } else {
            destory(start.cur);
            ++start;
        }
    }
    iterator erase(iterator pos) {
        if(pos == start) {
            pop_front();
            return start;
        } else if(pos == finish - 1) {
            pop_back();
            return finish;
        } else {
            difference_type n = pos - start;
            if(n < (size() / 2)) {
                std::copy_backward(start, pos, pos + 1);
                destory(start.cur);
                if (start.cur == start.last - 1) {
                    data_allocater::deallocate(start.first, buffer_size());
                }
                ++start;
            } else {
                std::copy(pos + 1, finish, pos);
                if(finish.cur == finish.first) {
                    data_allocater::deallocate(finish.first, buffer_size());
                }
                --finish;
                destory(finish.cur);
            }
            return start + n;
        }
    }
    iterator erase(iterator first, iterator last) {
        if(first == start && last == finish) {
            clear();
            return start;
        } else if(first == start) {
            map_pointer pre = first.node;
            destory(first.cur, last.cur);
            start = last;
            for(; pre < start.node; pre++) {
                data_allocater::deallocate(*pre, buffer_size());
            }
            return start;
        } else if(last == finish) {
            map_pointer end = last.node;
            destory(first.cur, last.cur);
            finish = first;
            for(map_pointer cur = finish.node + 1; cur <= end; cur++) {
                data_allocater::deallocate(*cur, buffer_size());
            }
            return finish;
        } else {
            difference_type gap = first - start;
            difference_type n = last - first;
            if(first - start < finish - last) {
                std::copy_backward(start, first, last);
                map_pointer pre = start.node;
                start += n;
                for(; pre < start.node; pre++) {
                    data_allocater::deallocate(*pre, buffer_size());
                }
            } else {
                std::copy(last, finish, first);
                destory((finish - n).cur, finish.cur);
                map_pointer pre = finish.node;
                finish -= n;
                for(map_pointer cur = finish.node + 1; cur <= pre; cur++) {
                    data_allocater::deallocate(*cur, buffer_size());
                }
            }
            return start + gap;
        }
    }
    void clear() {
        destory(start.cur, finish.cur);
        for(map_pointer cur = start.node + 1; cur < finish.node; cur++) {
            data_allocater::deallocate(*cur, buffer_size());
        }
        if(start.node != finish.node) {
            data_allocater::deallocate(finish.first, buffer_size());
        }
        finish = start;
    }
    iterator insert(iterator pos, const value_type &x) {
        if (pos == start) {
            push_front(x);
            return start ;
        } else if(pos == finish) {
            push_back(x);
            return finish - 1;
        } else {
            return insert_aux(pos, x);
        }
    }
    iterator insert(iterator pos) {
        return insert(pos, value_type());
    }
    iterator insert(iterator pos, size_type n, const value_type &x) {
        size_type add_nodes_back = (n > (finish.last - finish.cur - 1)) ? (n - (finish.last - finish.cur - 1) - 1) / buffer_size() + 1 : 0;
        size_type add_nodes_front = (n > (start.cur - start.first)) ? (n - (start.cur - start.first) - 1) / buffer_size() + 1 : 0;
        difference_type pres = pos - start;
        if(pres < size() / 2) {
            reserve_map_at_front(add_nodes_front);
            if(n >= size_type(pres)) {
                uninitialized_copy(start, pos, start - difference_type(n));
                if(n > size_type(pres)) {
                    uninitialized_fill(pos - difference_type(n), start, x);
                }
                std::fill(start, pos, x);
            } else {
                uninitialized_copy(start, start + difference_type(n), start - difference_type(n));
                std::copy(start + difference_type(n), pos, start);
                std::fill(pos - difference_type(n), pos, x);
            }
            start -= difference_type(n);
        } else {
            reserve_map_at_back(add_nodes_back);
            difference_type after = finish - pos;
            if (n >= size_type(after)) {
                uninitialized_copy(pos, finish, pos + difference_type(n));
                if(n > size_type(after)) {
                    uninitialized_fill(finish, pos + difference_type(n), x);
                }
                std::fill(pos, finish, x);
            } else {
                uninitialized_copy(finish - difference_type(n), finish, finish);
                std::copy_backward(pos, finish - difference_type(n), finish);
                std::fill(pos, pos + difference_type(n), x);
            }
            finish += difference_type(n);
        }
        return start + pres;
    }
    iterator insert(iterator pos, int n, const value_type &x) {
        return insert(pos, size_type(n), x);
    }
    iterator insert(iterator pos, long n, const value_type &x) {
        return insert(pos, size_type(n), x);
    }
    void resize(size_type new_size, const value_type &x) {
        if(new_size > size()) {
            insert(finish, new_size - size(), x);
        } else if(new_size < size()) {
            erase(finish - difference_type(size() - new_size), finish);
        }
    }
    void resize(size_type new_size) {
        resize(new_size, value_type());
    }
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        insert(pos, first, last, category_type(first));
    }
    bool operator== (const deque &x) {
        return size() == x.size() && std::equal(begin(), end(), x.begin());
    }
    bool operator!= (const deque &x) {
        return size() != x.size() || !std::equal(begin(), end(), x.begin());
    }
    bool operator< (const deque &x) {
        iterator first = begin(), first_x = x.begin();
        while(first != end() && first_x != x.end()) {
            if(*first < *first_x) {
                return true;
            } else if(*first > *first_x) {
                return false;
            }
        }
        return first_x != x.end();
    }
    void display() {
        for(value_type val : *this) {
            std::cout << val << " ";
        }
        std::cout << std::endl;
    }
protected:
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last, input_iterator_tag) {
        std::copy(first, last, inserter(*this, pos));
    }
    template <class ForwardIterator>
    void insert(iterator pos, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        size_type dis = distance(first, last);
        size_type add_nodes_back = (dis > (finish.last - finish.cur - 1)) ? (dis - (finish.last - finish.cur - 1) - 1) / buffer_size() + 1 : 0;
        size_type add_nodes_front = (dis > (start.cur - start.first)) ? (dis - (start.cur - start.first) - 1) / buffer_size() + 1 : 0;
        difference_type pres = pos - start;
        if(pres < size() / 2) {
            reserve_map_at_front(add_nodes_front);
            if(dis >= size_type(pres)) {
                uninitialized_copy(start, pos, start - difference_type(dis));
                if(dis > size_type(pres)) {
                    uninitialized_copy(first, first + (dis - pres), pos - difference_type(dis));
                }
                std::copy(first + (dis - pres), last, start);
            } else {
                uninitialized_copy(start, start + difference_type(dis), start - difference_type(dis));
                std::copy(start + difference_type(dis), pos, start);
                std::copy(first, last, pos - difference_type(dis));
            }
            start -= difference_type(dis);
        } else {
            reserve_map_at_back(add_nodes_back);
            difference_type after = finish - pos;
            if (dis >= size_type(after)) {
                uninitialized_copy(pos, finish, pos + difference_type(dis));
                if(dis > size_type(after)) {
                    uninitialized_copy(last - (dis - after), last, finish);
                }
                std::copy(first, last - (dis - after), pos);
            } else {
                uninitialized_copy(finish - difference_type(dis), finish, finish);
                std::copy_backward(pos, finish - difference_type(dis), finish);
                std::copy(first, last, pos);
            }
            finish += difference_type(dis);
        }
    }
    template <class InputIterator>
    void range_initialize(InputIterator first, InputIterator last, input_iterator_tag) {
        for(; first != last; first++) {
            push_back(*first);
            +first;
        }
    }
    template <class ForwardIterator>
    void range_initialize(ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
        size_type n = distance(first, last);
        create_map_and_nodes(n);
        uninitialized_copy(first, last, start);
    }
    void fill_initialize(size_type n, const value_type &x) {
        create_map_and_nodes(n);
        uninitialized_fill(start, finish, x);
    }
    iterator insert_aux(iterator pos, const value_type &x) {
        difference_type n = pos - start;
        if(n < size() / 2) {
            push_front(front());
            std::copy(start + 2, pos, start + 1);
            --pos;
        } else {
            push_back(back());
            std::copy_backward(pos, finish - 2, finish - 1);
        }
        *pos = x;
        return pos;
    }
    void push_front_aux(const value_type &x) {
        reserve_map_at_front();
        *(start.node - 1) = data_allocater::allocate(buffer_size());
        --start;
        construct(start.cur, x);
    }
    void push_back_aux(const value_type &x) {
        reserve_map_at_back();
        construct(finish.cur, x);
        *(finish.node + 1) = data_allocater::allocate(buffer_size());
        ++finish;
    }
    void reserve_map_at_front(size_type n = 1) {
        if(start.node - map < n) {
            if(map_size - (finish.node - start.node + 1 ) >= n) {
                difference_type gap = n - (start.node - map);
                std::copy_backward(start.node, finish.node + 1, finish.node + gap + 1);
                start.set_node(start.node + gap);
                finish.set_node(finish.node + gap);
                for(map_pointer cur = map; cur < start.node; cur++) {
                    *cur = data_allocater::allocate(buffer_size());
                }
            } else {
                reallocate_map(n, true);
            }
        } else {
            map_pointer cur = start.node - 1;
            while(n > 0) {
                *cur = data_allocater::allocate(buffer_size());
                --n;
                --cur;
            }
        }
    }
    void reserve_map_at_back(size_type n = 1) {
        if(buffer_size() - (finish.node - map) - 1 < n) {
            if(map_size - (finish.node - start.node + 1 ) >= n) {
                difference_type gap = n - (buffer_size() - (finish.node - map) - 1);
                std::copy(start.node, finish.node + 1, start.node - gap);
                start.set_node(start.node - gap);
                finish.set_node(finish.node - gap);
                for(map_pointer cur = finish.node + 1; cur < map + map_size; cur++) {
                    *cur = data_allocater::allocate(buffer_size());
                }
            } else {
                reallocate_map(n, false);
            }
        } else {
            map_pointer cur = finish.node + 1;
            while(n > 0) {
                *cur = data_allocater::allocate(buffer_size());
                ++cur;
                --n;
            }
        }
    }
    void reallocate_map(size_type n, bool add_front) {
        size_type old_size = finish.node - start.node + 1;
        size_type new_map_size = map_size + std::max(map_size, old_size + n + 2);
        map_pointer new_map, new_start, new_finish;
        try {
            new_map = map_allocater::allocate(new_map_size);
        } catch(...) {
            map_allocater::deallocate(new_map, new_map_size);
            throw;
        }
        new_start = new_map + (new_map_size - old_size) / 2 + (add_front ? n : 0);
        std::copy(start.node, finish.node + 1, new_start);
        map_allocater::deallocate(map, map_size);
        map = new_map;
        map_size = new_map_size;
        start.set_node(new_start);
        finish.set_node(new_start + old_size - 1);
        map_pointer cur = (add_front ? start.node - 1 : finish.node + 1);
        while(n > 0) {
            *cur = data_allocater::allocate(buffer_size());
            --n;
            add_front ? --cur : ++cur;
        }
    }
    void pop_front_aux() {
        destory(start.cur);
        data_allocater::deallocate(start.first);
        start.set_node(start.node + 1);
        start.cur = start.first;
    }
    void pop_back_aux() {
        data_allocater::deallocate(finish.first);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        destory(finish.cur);
    }
    void create_map_and_nodes(size_type n) {
        size_type node_size = n / buffer_size() + 1;
        size_type new_map_size = std::max(initial_map_size(), node_size + 2);
        map_pointer new_map;
        iterator new_start, new_finish;
        try {
            new_map = map_allocater::allocate(new_map_size);
            map_pointer cur = new_map + (new_map_size - node_size) / 2;
            for(map_pointer tmp = cur; tmp < (cur + node_size); tmp++) {
                *tmp = data_allocater::allocate(buffer_size());
            }
            new_start.set_node(cur);
            new_finish.set_node(cur + node_size - 1);
        } catch(...) {
            for(map_pointer tmp = new_start.node; tmp <= new_finish.node; tmp++) {
                data_allocater::deallocate(*tmp, buffer_size());
            }
            map_allocater::deallocate(new_map, new_map_size);
            throw;
        }
        map = new_map;
        map_size = new_map_size;
        start = new_start;
        start.cur = start.first;
        finish = new_finish;
        finish.cur = finish.first + n % buffer_size();
    }
    void destory_map_and_nodes() {
        for(map_pointer cur = start.node; cur <= finish.node; cur++) {
            data_allocater::deallocate(*cur, buffer_size());
        }
        map_allocater::deallocate(map, map_size);
    }

};

}

#endif
