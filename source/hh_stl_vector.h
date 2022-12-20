#ifndef __HH_STL_VECTOR_H_
#define __HH_STL_VECTOR_H_

#include<cstddef>
#include<algorithm>
#include "hh_stl_iterator.h"
#include "hh_stl_alloc.h"
#include "hh_stl_uninitialized.h"

namespace hh_std {

template <class T, class Alloc = alloc>  // jjhou: alloc defined in stl_alloc.h
class vector {
public:
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type* iterator;
    typedef const value_type* const_iterator;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;

    typedef reverse_iterator<const_iterator>  const_r_iterator;
    typedef reverse_iterator<iterator> r_iterator;
protected:
    typedef simple_alloc<value_type, Alloc> data_allocator;
    iterator start;
    iterator finish;
    iterator end_of_storage;
    void insert_aux(iterator position, const T& x);
    void deallocate() {
        if(start) {
            data_allocator::deallocate(start, (end_of_storage - start));
        }
    }

    void fill_initialize(size_type n, const T& value) {
        start = allocate_and_fill(n, value);
        finish = end_of_storage = start + n;
    }
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
        return r_iterator (end());
    }
    const_r_iterator  rbegin() const {
        return const_r_iterator (end());
    }
    r_iterator  rend() {
        return r_iterator (begin());
    }
    const_r_iterator  rend() const {
        return const_r_iterator (begin());
    }
    size_type size() const {
        return (finish - start);
    }
    size_type max_size() const {
        return (size_type(-1) / sizeof(T));
    }
    size_type capacity() const {
        return (end_of_storage - start);
    }
    bool empty() const {
        return finish == start;
    }
    reference operator[](size_type n) {
        return *(start + n);
    }
    const_reference operator[](size_type n) const {
        return *(start + n);
    }

    vector() : start(0), finish(0), end_of_storage(0) {}
    vector(size_type n, const T& value) {
        fill_initialize(n, value);
    }
    vector(int n, const T& value) {
        fill_initialize(n, value);
    }
    vector(long n, const T& value) {
        fill_initialize(n, value);
    }
    explicit vector(size_type n) {
        fill_initialize(n, T());
    }

    vector(const vector<T, Alloc>& x) {
        range_initialize(x.begin(), x.end(), category_type(x.begin()));
    }
#ifdef __STL_MEMBER_TEMPLATES
    template <class InputIterator>
    vector(InputIterator first, InputIterator last) :
        start(0), finish(0), end_of_storage(0) {
        range_initialize(first, last, category_type(first));
    }
#else /* __STL_MEMBER_TEMPLATES */
    vector(const_iterator first, const_iterator last) {
        range_initialize(first, last, category_type(first));
    }
#endif /* __STL_MEMBER_TEMPLATES */
    ~vector() {
        destory(start, end_of_storage);
        deallocate();
    }
    vector<T, Alloc>& operator=(const vector<T, Alloc>& x);
    void reserve(size_type n) {
        if(n > capacity()) {
            iterator tmp;
            size_type old = size();
            try {
                tmp = allocate_and_copy(n, start, finish);
            } catch(...) {
                destory(tmp, tmp + old);
                data_allocator::deallocate(tmp, n);
                throw;
            }
            destory(start, finish);
            deallocate();
            start = tmp;
            finish = start + old;
            end_of_storage = start + n;
        }
    }
    reference front() {
        return *begin();
    }
    const_reference front() const {
        return *begin();
    }
    reference back() {
        return *(end() - 1);
    }
    const_reference back() const {
        return *(end() - 1);
    }
    void push_back(const T& x) {
        if(finish != end_of_storage) {
            construct(finish++, x);
        } else {
            insert_aux(finish, x);
        }
    }
    void swap(vector<T, Alloc>& x) {
        std::swap(start, x.start);
        std::swap(finish, x.finish);
        std::swap(end_of_storage, x.end_of_storage);
    }
    iterator insert(iterator position, const T& x) {
        size_type n = position - start;
        if (finish != end_of_storage && position == finish) {
            construct(finish++, x);
        } else {
            insert_aux(position, x);
        }
        return start + n;
    }
    iterator insert(iterator position) {
        return insert(position, T());
    }
#ifdef __STL_MEMBER_TEMPLATES
    template <class InputIterator>
    void insert(iterator position, InputIterator first, InputIterator last) {
        range_insert(position, first, last, category_type(first));
    }
#else /* __STL_MEMBER_TEMPLATES */
    void insert(iterator position,
                const_iterator first, const_iterator last);
#endif /* __STL_MEMBER_TEMPLATES */

    void insert (iterator pos, size_type n, const T& x);
    void insert (iterator pos, int n, const T& x) {
        insert(pos, size_type(n), x);
    }
    void insert (iterator pos, long n, const T& x) {
        insert(pos, size_type(n), x);
    }

    void pop_back() {
        finish--;
        destory(finish);
    }
    iterator erase(iterator position) {
        if((position + 1) != finish)
            std::copy(position + 1, finish, position);
        destory(--finish);
        return position;
    }
    iterator erase(iterator first, iterator last) {
        if(last != finish)
            std::copy(last, finish, first);
        difference_type n = distance(first, last);
        while(n > 0) {
            destory(--finish);
            n--;
        }
        return first;
    }
    void resize(size_type new_size, const T& x) {
        if (new_size < size()) {
            erase(finish - (size() - new_size), finish);
        } else if(new_size > size()) {
            insert(finish, new_size - size(), x);
        }
    }
    void resize(size_type new_size) {
        resize(new_size, T());
    }
    void clear() {
        destory(start, finish);
        finish = start;
    }

protected:
    iterator allocate_and_fill(size_type n, const T& x) {
        iterator unini = data_allocator::allocate(n);
        uninitialized_fill(unini, unini + n, x);
        return unini;
    }

#ifdef __STL_MEMBER_TEMPLATES
    template <class ForwardIterator>
    iterator allocate_and_copy(size_type n,
                               ForwardIterator first, ForwardIterator last) {
        iterator unini = data_allocator::allocate(n);
        uninitialized_copy(first, last, unini);
        return unini;
    }
#else /* __STL_MEMBER_TEMPLATES */
    iterator allocate_and_copy(size_type n,
                               const_iterator first, const_iterator last) {
        iterator unini = data_allocator::allocate(n);
        uninitialized_copy(first, last, unini);
        return unini;
    }
#endif /* __STL_MEMBER_TEMPLATES */


#ifdef __STL_MEMBER_TEMPLATES
    template <class InputIterator>
    void range_initialize(InputIterator first, InputIterator last,
                          input_iterator_tag) {
        for(; first != last; first++) {
            push_back(*first);
        }
    }

    // This function is only called by the constructor.  We have to worry
    //  about resource leaks, but not about maintaining invariants.
    template <class ForwardIterator>
    void range_initialize(ForwardIterator first, ForwardIterator last,
                          forward_iterator_tag) {
        size_type n = last - first;
        start = allocate_and_copy(n, first, last);
        finish = start + n;
        end_of_storage = finish;
    }

    template <class InputIterator>
    void range_insert(iterator pos,
                      InputIterator first, InputIterator last,
                      input_iterator_tag);

    template <class ForwardIterator>
    void range_insert(iterator pos,
                      ForwardIterator first, ForwardIterator last,
                      forward_iterator_tag);

#endif /* __STL_MEMBER_TEMPLATES */
};

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::range_insert(iterator pos, InputIterator first, InputIterator last, input_iterator_tag) {
    for(; first != last; first++) {
        pos = insert(pos, *first);
        pos++;
    }
}

template <class T, class Alloc>
template <class ForwardIterator>
void vector<T, Alloc>::range_insert(iterator pos, ForwardIterator first, ForwardIterator last, forward_iterator_tag) {
    size_type insert_size = distance(first, last);
    if(insert_size > (end_of_storage - finish)) {
        size_type old_size = size();
        size_type new_size = old_size + (old_size > insert_size ? old_size : insert_size);
        iterator new_start, new_finish;
        try {
            new_start = data_allocator::allocate(new_size);
            new_finish = uninitialized_copy(start, pos, new_start);
            new_finish = uninitialized_copy(first, last, new_finish);
            new_finish = uninitialized_copy(pos, finish, new_finish);
        } catch(...) {
            destory(new_start, new_finish);
            data_allocator::deallocate(new_start, new_size);
            throw;
        }
        destory(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start + new_size;
    } else {
        if (pos == finish) {
            finish = uninitialized_copy(first, last, finish);
        } else {
            size_type old_right_size = finish - pos;
            if(old_right_size <= insert_size) {
                uninitialized_copy(pos, finish, pos + insert_size);
                if(old_right_size == insert_size) {
                    std::copy(first, last, pos);
                } else {
                    std::copy(first, first + old_right_size, pos);
                    uninitialized_copy(first + old_right_size, last, finish);
                }
            } else {
                uninitialized_copy(finish - insert_size, finish, finish);
                std::copy(pos, finish - insert_size, finish - (old_right_size - insert_size));
                std::copy(first, last, pos);
            }
            finish += insert_size;
        }
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T &x) {
    if(n > (end_of_storage - finish)) {
        size_type old_size = size();
        size_type new_size = old_size + (old_size > n ? old_size : n);
        iterator new_start, new_finish;
        try {
            new_start = data_allocator::allocate(new_size);
            new_finish = uninitialized_copy(start, pos, new_start);
            uninitialized_fill_n(new_finish, n, x);
            new_finish += n;
            new_finish = uninitialized_copy(pos, finish, new_finish);
        } catch(...) {
            destory(new_start, new_finish);
            data_allocator::deallocate(new_start, new_size);
            throw;
        }
        destory(start, finish);
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = start + new_size;
    } else {
        if (pos == finish) {
            uninitialized_fill_n(finish, n, x);
            finish += n;
        } else {
            size_type old_right_size = finish - pos;
            if(old_right_size <= n) {
                uninitialized_copy(pos, finish, pos + n);
                if(old_right_size == n) {
                    std::fill_n(pos, n, x);
                } else {
                    std::fill_n(pos, old_right_size, x);
                    uninitialized_fill_n(finish, n - old_right_size, x);
                }
            } else {
                uninitialized_copy(finish - n, finish, finish);
                std::copy(pos, finish - n, finish - (old_right_size - n));
                std::fill_n(pos, n, x);
            }
            finish += n;
        }
    }
}

#ifndef __STL_MEMBER_TEMPLATES
template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, const_iterator first, const_iterator last) {
    range_insert(pos, first, last, category_type(first));
}
#endif
template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T &x) {
    if(finish != end_of_storage) {
        if(position == finish) {
            construct(finish++, x);
        } else {
            construct(finish, finish - 1);
            std::copy(position, finish, position + 1);
            finish++;
            destory(position);
            construct(position, x);
        }
    } else {
        iterator new_start, new_finish;
        size_type new_capacity = (capacity() == 0 ? 1 : 2 * capacity());
        try {
            new_start = allocate_and_copy(new_capacity, start, finish);
            new_finish = new_start + size();
            construct(new_finish++, x);
        } catch(...) {
            destory(new_start, new_finish);
            data_allocator::deallocate(new_start, capacity() * 2);
            throw;
        }
        end_of_storage = new_start + new_capacity;
        start = new_start;
        finish = new_finish;
    }
}

template <class T, class Alloc>
inline bool operator== (const vector<T, Alloc> &a, const vector<T, Alloc> &b) {
    if(a.size() != b.size())
        return false;
    for(typename vector<T, Alloc>::size_type i = 0; i < a.size(); i++) {
        if(a[i] != b[i])
            return false;
    }
    return true;
}

template <class T, class Alloc>
inline bool operator!= (const vector<T, Alloc> &a, const vector<T, Alloc> &b) {
    if(a.size() != b.size())
        return true;
    for(typename vector<T, Alloc>::size_type i = 0; i < a.size(); i++) {
        if(a[i] != b[i])
            return true;
    }
    return false;
}

template <class T, class Alloc>
inline bool operator< (const vector<T, Alloc> &a, const vector<T, Alloc> &b) {
    typename vector<T, Alloc>::size_type cur = 0;
    while(cur < a.size() && cur < b.size()) {
        if(a[cur] < b[cur])
            return true;
        else if(a[cur] > b[cur])
            return false;
        cur++;
    }
    if(cur < a.size()) {
        return false;
    } else if(cur < b.size()) {
        return true;
    }
    return false;
}

template <class T, class Alloc>
inline void swap (vector<T, Alloc> &a, vector<T, Alloc> &b) {
    a.swap(b);
}

template <class T, class Alloc>
vector<T, Alloc> &vector<T, Alloc>::operator= (const vector<T, Alloc> &x) {
    if(&x != this) {
        if (x.size() > capacity()) {
            iterator new_start;
            try {
                new_start = allocate_and_copy(x.size(), x.begin(), x.end());
            } catch(...) {
                destory(new_start, new_start + x.size());
                data_allocator::deallocate(new_start, x.size());
                throw;
            }
            destory(start, finish);
            deallocate();
            start = new_start;
            end_of_storage = finish = start + x.size();
        } else if(x.size() > size()) {
            std::copy(x.begin(), x.begin() + size(), start);
            finish = uninitialized_copy(x.begin() + size(), x.end(), finish);
        } else {
            std::copy(x.begin(), x.end(), start);
            while(size() > x.size()) {
                destory(--finish);
            }
        }
    }
    return *this;
}

}

#endif
