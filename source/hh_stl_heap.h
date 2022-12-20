#ifndef __HH_STL_HEAP_H_
#define __HH_STL_HEAP_H_

#include "hh_stl_iterator.h"

namespace hh_std {

template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    if ((last - first) < 2) return;
    Distance hole_index = (last - first) - 1;
    Distance parent_index = (hole_index - 1) / 2;
    T value = *(last - 1);
    while(hole_index > 0 && *(first + parent_index) < value) {
        *(first + hole_index) = *(first + parent_index);
        hole_index = parent_index;
        parent_index = (hole_index - 1) / 2;
    }
    *(first + hole_index) = value;
}

template <class RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __push_heap(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __push_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    if ((last - first) < 2) return;
    Distance hole_index = (last - first) - 1;
    Distance parent_index = (hole_index - 1) / 2;
    T value = *(last - 1);
    while(hole_index > 0 && comp(*(first + parent_index)), value) {
        *(first + hole_index) = *(first + parent_index);
        hole_index = parent_index;
        parent_index = (hole_index - 1) / 2;
    }
    *(first + hole_index) = value;
}

template <class RandomAccessIterator, class Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __push_heap(first, last, distance_type(first), value_type(first), comp);
}

template <class RandomAccessIterator, class Distance, class T>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    if ((last - first) < 2) return;
    T value = *(last - 1);
    *(last - 1) = *(first);
    Distance len = (last - first) - 1;
    Distance hole_index = 0;
    Distance next_index = hole_index * 2 + 2;
    while(next_index < len) {
        if(*(first + next_index) < * (first + next_index - 1)) {
            next_index -= 1;
        }
        if(*(first + next_index) > value) {
            *(first + hole_index) = *(first + next_index);
            hole_index = next_index;
            next_index = 2 * hole_index + 2;
        } else {
            break;
        }
    }
    if(next_index == len && *(first + next_index - 1) > value) {
        *(first + hole_index) = *(first + next_index - 1);
        hole_index = next_index - 1;
    }
    *(first + hole_index) = value;
}

template <class RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    __pop_heap(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    if ((last - first) < 2) return;
    T value = *(last - 1);
    *(last - 1) = *(first);
    Distance len = (last - first) - 1;
    Distance hole_index = 0;
    Distance next_index = hole_index * 2 + 2;
    while(next_index < len) {
        if(comp(*(first + next_index), *(first + next_index - 1))) {
            next_index -= 1;
        }
        if(comp(value, *(first + next_index))) {
            *(first + hole_index) = *(first + next_index);
            hole_index = next_index;
            next_index = 2 * hole_index + 2;
        } else {
            break;
        }
    }
    if(next_index == len && comp(value, *(first + next_index - 1))) {
        *(first + hole_index) = *(first + next_index - 1);
        hole_index = next_index - 1;
    }
    * (first + hole_index) = value;
}

template <class RandomAccessIterator, class Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    __pop_heap(first, last, distance_type(first), value_type(first), comp);
}

template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance len, Distance hole_index,  Distance*, T*) {
    Distance next_index = hole_index * 2 + 2;
    T value = *(first + hole_index);
    while(next_index < len) {
        if(*(first + next_index) < * (first + next_index - 1)) {
            next_index--;
        }
        if(*(first + next_index) > value) {
            *(first + hole_index) = *(first + next_index);
            hole_index = next_index;
            next_index = hole_index * 2 + 2;
        } else {
            break;
        }
    }
    if(next_index == len && *(first + next_index - 1) > value) {
        *(first + hole_index) = *(first + next_index - 1);
        hole_index = next_index - 1;
    }
    *(first + hole_index) = value;
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __adjust_heap(RandomAccessIterator first, Distance len, Distance hole_index, Distance*, T*, Compare comp) {
    Distance next_index = hole_index * 2 + 2;
    T value = *(first + hole_index);
    while(next_index < len) {
        if(comp(*(first + next_index), *(first + next_index - 1))) {
            next_index--;
        }
        if(comp(value, *(first + next_index))) {
            *(first + hole_index) = *(first + next_index);
            hole_index = next_index;
            next_index = hole_index * 2 + 2;
        } else {
            break;
        }
    }
    if(next_index == len && comp(value, *(first + next_index - 1))) {
        *(first + hole_index) = *(first + next_index - 1);
        hole_index = next_index - 1;
    }
    *(first + hole_index) = value;
}

template <class RandomAccessIterator, class Distance, class T>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    typedef Distance* distance_point;
    typedef T* value_point;
    Distance hole_index = (last - first - 2) / 2;
    while(hole_index >= 0) {
        __adjust_heap(first, last - first, hole_index, distance_point(), value_point());
        --hole_index;
    }
}

template <class RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last) {
    if((last - first) < 2) return;
    __make_heap(first, last, distance_type(first), value_type(first));
}

template <class RandomAccessIterator, class Distance, class T, class Compare>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    typedef Distance* distance_point;
    typedef T* value_point;
    Distance hole_index = (last - first - 2) / 2;
    while(hole_index >= 0) {
        __adjust_heap(first, last - first, hole_index, distance_point(), value_point(), comp);
        --hole_index;
    }
}

template <class RandomAccessIterator, class Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    if((last - first) < 2) return;
    __make_heap(first, last, distance_type(first), value_type(first), comp);
}

template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last) {
    while((last - first) > 1) {
        pop_heap(first, last);
        --last;
    }
}

template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    while((last - first) > 1) {
        pop_heap(first, last, comp);
        --last;
    }
}

};

#endif
