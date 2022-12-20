#ifndef __HH_STL_ITERATOR_H_
#define __HH_STL_ITERATOR_H_

#include<cstddef>
#include<iostream>

namespace hh_std {

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag: public input_iterator_tag {};
struct bidirectional_iterator_tag: public forward_iterator_tag {};
struct random_access_iterator_tag: public bidirectional_iterator_tag {};

template <typename Category, typename T, typename Distance = std::ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator {
    typedef Category iterator_category;
    typedef T value_type;
    typedef Distance difference_type;
    typedef Pointer pointer;
    typedef Reference reference;
};

template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type value_type;
    typedef typename Iterator::difference_type difference_type;
    typedef typename Iterator::pointer pointer;
    typedef typename Iterator::reference reference;
};

template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;
};

template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef const T* pointer;
    typedef const T& reference;
};

template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category category_type(const Iterator&) {
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

template <class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type* distance_type(const Iterator&) {
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type  __distance(InputIterator first, InputIterator last, input_iterator_tag) {
    typedef typename iterator_traits<InputIterator>::difference_type distance;
    distance n = 0;
    while(first != last) {
        ++n;
        ++first;
    }
    return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    return last - first;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
    return __distance(first, last, category_type(first));
}

template <class InputIterator, class Distance>
inline void __advance(InputIterator &first, Distance distance, input_iterator_tag) {
    while(distance) {
        first++;
        distance--;
    }
}

template <class InputIterator, class Distance>
inline void __advance(InputIterator &first, Distance distance, bidirectional_iterator_tag) {
    if (distance > 0) {
        while(distance) {
            ++first;
            distance--;
        }
    } else {
        while(distance) {
            --first;
            distance++;
        }
    }
}

template <class InputIterator, class Distance>
inline void __advance(InputIterator &first, Distance distance, random_access_iterator_tag) {
    first += distance;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator &first, Distance distance) {
    __advance(first, distance, category_type(first));
}

template <class BidirIterator>
BidirIterator prev(BidirIterator it, typename iterator_traits<BidirIterator>::difference_type n = 1) {
    advance(it, -n);
    return it;
}

//template <class It, class Value, class Reference, class Distance = ptrdiff_t>
//struct reverse_iterator {
//protected:
//    It current = It();
//public:
//    typedef Value value_type;
//    typedef Reference reference;
//    typedef Distance difference_type;
//    reverse_iterator() = default;
//    reverse_iterator(It it): current(it) {}
//    It base() {
//        return current;
//    }
//    value_type operator*() {
//        return *prev(current);
//    }
//    reverse_iterator& operator++() {
//        --current;
//        return *this;
//    }
//    reverse_iterator operator++(int) {
//        reverse_iterator tmp = *this;
//        ++(*this);
//        return tmp;
//    }
//    reverse_iterator& operator--() {
//        ++current;
//        return *this;
//    }
//    reverse_iterator operator--(int) {
//        reverse_iterator tmp = *this;
//        --(*this);
//        return tmp;
//    }
//    bool operator== (const reverse_iterator &rit) {
//        return current == rit.current;
//    }
//    bool operator!= (const reverse_iterator &rit) {
//        return current != rit.current;
//    }
//};


template <class Iterator>
class reverse_iterator {
protected:
    Iterator current;
public:
    typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
    typedef typename iterator_traits<Iterator>::value_type value_type;
    typedef typename iterator_traits<Iterator>::pointer pointer;
    typedef typename iterator_traits<Iterator>::reference reference;
    typedef typename iterator_traits<Iterator>::difference_type difference_type;

    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> self;

    reverse_iterator() = default;
    explicit reverse_iterator(iterator_type x): current(x) {}
    reverse_iterator(const reverse_iterator &x): current(x.base()) {}

    iterator_type base() const {
        return current;
    }

    reference &operator* () const {
        return *prev(current);
    }

    pointer operator-> () const {
        return &(operator*());
    }

    self &operator++ () {
        --current;
        return *this;
    }

    self operator++ (int) {
        self tmp = *this;
        --current;
        return tmp;
    }

    self &operator-- () {
        ++current;
        return *this;
    }

    self operator-- (int) {
        self tmp = *this;
        ++current;
        return tmp;
    }

    self operator+ (difference_type diff) const {
        self tmp = *this;
        tmp.current -= diff;
        return tmp;
    }

    self &operator+= (difference_type diff) {
        current -= diff;
        return *this;
    }

    self operator- (difference_type diff) const {
        self tmp = *this;
        tmp.current += diff;
        return tmp;
    }

    self &operator-= (difference_type diff) {
        current += diff;
        return *this;
    }

    value_type &operator[] (size_t n) const {
        return *(*this + n);
    }

    bool operator!= (const self &x) const {
        return current != x.current;
    }

};

template <class Iterator>
inline bool operator== (const reverse_iterator<Iterator> &a, const reverse_iterator<Iterator> &b) {
    return a.base() == b.base();
}

template <class Iterator>
inline bool operator< (const reverse_iterator<Iterator> &a, const reverse_iterator<Iterator> &b) {
    return a.base() < b.base();
}

template <class Iterator>
inline typename reverse_iterator<Iterator>::difference_type operator- (const reverse_iterator<Iterator> &a, const reverse_iterator<Iterator> &b) {
    return a.base() - b.base();
}

template <class Container>
struct front_insert_iterator {
protected:
    Container *container;
public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void size_type;
    typedef void pointer;
    typedef void reference;
public:
    front_insert_iterator() = default;
    front_insert_iterator(Container &con): container(&con) {}
    front_insert_iterator &operator++ () {
        return *this;
    }
    front_insert_iterator &operator++ (int) {
        return *this;
    }
    front_insert_iterator &operator+ (int) {
        return *this;
    }
    front_insert_iterator &operator* () {
        return *this;
    }
    front_insert_iterator &operator= (const typename Container::value_type &x) {
        container->push_front(x);
        return *this;
    }
};

template <class Container>
struct back_insert_iterator {
protected:
    Container *container;
public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void size_type;
    typedef void pointer;
    typedef void reference;
public:
    back_insert_iterator() = default;
    back_insert_iterator(Container &con): container(&con) {}
    back_insert_iterator &operator++ () {
        return *this;
    }
    back_insert_iterator &operator++ (int) {
        return *this;
    }
    back_insert_iterator &operator+ (int) {
        return *this;
    }
    back_insert_iterator &operator* () {
        return *this;
    }
    back_insert_iterator &operator= (const typename Container::value_type &x) {
        container->push_back(x);
        return *this;
    }
    back_insert_iterator &operator() (const typename Container::value_type &x) {
        container->push_back(x);
        return *this;
    }
};

template <class Container>
back_insert_iterator<Container> back_inserter(Container &c) {
    return back_insert_iterator<Container>(c);
}

template <class Container>
front_insert_iterator<Container> front_inserter(Container &c) {
    return front_insert_iterator<Container>(c);
}

template <class Container>
struct insert_iterator {
protected:
    Container *container;
    typename Container::iterator iter;
public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void size_type;
    typedef void pointer;
    typedef void reference;
public:
    insert_iterator() = default;
    insert_iterator(Container &con, const typename Container::iterator &i): container(&con), iter(i) {}
    insert_iterator &operator++ () {
        return *this;
    }
    insert_iterator &operator++ (int) {
        return *this;
    }
    insert_iterator &operator+ (int) {
        return *this;
    }
    insert_iterator &operator* () {
        return *this;
    }
    insert_iterator &operator= (const typename Container::value_type &x) {
        iter = container->insert(iter, x);
        ++iter;
        return *this;
    }
};

template <class Container>
insert_iterator<Container> inserter(Container &c, const typename Container::iterator iter) {
    return insert_iterator<Container>(c, iter);
}

template <class T, class Distance>
struct istream_iterator;

template <class T, class Distance>
bool operator== (const istream_iterator<T, Distance> &x, const istream_iterator<T, Distance> &y) {
    return x.status == y.status && (!x.status || x.stream == y.stream);
}

template <class T, class Distance>
bool operator!= (const istream_iterator<T, Distance> &x, const istream_iterator<T, Distance> &y) {
    return !(x.status == y.status && (!x.status || x.stream == y.stream));
}

template <class T, class Distance = ptrdiff_t>
struct istream_iterator {
protected:
    std::istream *stream;
    T value;
    bool status;
    void read() {
        status = *stream ? true : false;
        if(status) {
            *stream >> value;
        }
        status = *stream ? true : false;
    }
public:
    typedef input_iterator_tag iterator_category;
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef ptrdiff_t difference_type;
    typedef size_t size_type;

public:
    istream_iterator(): stream(&std::cin), status(false) {}
    istream_iterator(std::istream &i): stream(&i), status(false) {
        read();
    }
    reference operator* () const {
        return (reference)value;
    }
    pointer operator-> () const {
        return &(operator*());
    }
    istream_iterator &operator++ () {
        read();
        return *this;
    }
    istream_iterator operator++ (int) {
        istream_iterator tmp = *this;
        read();
        return tmp;
    }
    friend bool operator== <> (const istream_iterator &x, const istream_iterator &y);
    friend bool operator!= <> (const istream_iterator &x, const istream_iterator &y);
};

template <class T>
struct ostream_iterator {
protected:
    std::ostream *stream;
    const char *string;
public:
    typedef output_iterator_tag iterator_category;
    typedef void value_type;
    typedef void pointer;
    typedef void reference;
    typedef void difference_type;
    typedef void size_type;
public:
    ostream_iterator(): stream(&std::cout) {}
    ostream_iterator(std::ostream &o): stream(&o) {}
    ostream_iterator(std::ostream &o, const char *s): stream(&o), string(s) {}
    ostream_iterator &operator* () {
        return *this;
    }
    ostream_iterator &operator++ () {
        return *this;
    }
    ostream_iterator operator++ (int) {
        return *this;
    }
    ostream_iterator &operator= (const T &x) {
        *stream << x;
        if(string) *stream << string;
        return *this;
    }

};

};

#endif
