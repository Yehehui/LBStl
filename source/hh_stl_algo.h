#ifndef __HH_STL_ALGO_H_
#define __HH_STL_ALGO_H_

#include "hh_stl_algobase.h"
#include "hh_stl_iterator.h"
#include "hh_stl_heap.h"
#include<cstddef>

namespace hh_std {

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output) {
    while(first != last && first_x != last_x) {
        if(*first < *first_x) {
            *output = *first;
            ++first;
        } else if(*first_x < *first) {
            *output = *first_x;
            ++first_x;
        } else {
            ++first;
        }
    }
    while(first != last) {
        *output = *first;
        ++first;
    }
    while(first_x != last_x) {
        *output = *first_x;
        ++first_x;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOps>
OutputIterator set_union(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output, BinaryOps op) {
    while(first != last && first_x != last_x) {
        if(op(*first, *first_x)) {
            *output = *first;
            ++first;
        } else if(op(*first_x, *first)) {
            *output = *first_x;
            ++first_x;
        } else {
            ++first;
        }
        ++output;
    }
    while(first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    while(first_x != last_x) {
        *output = *first_x;
        ++first_x;
        ++output;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output) {
    while(first != last && first_x != last_x) {
        if(*first < *first_x) {
            ++first;
        } else if(*first_x < *first) {
            ++first_x;
        } else {
            *output = *first;
            ++first;
            ++first_x;
            ++output;
        }
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOps>
OutputIterator set_intersection(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output, BinaryOps op) {
    while(first != last && first_x != last_x) {
        if(op(*first, *first_x)) {
            ++first;
        } else if(op(*first_x, *first)) {
            ++first_x;
        } else {
            *output = *first;
            ++first;
            ++first_x;
            ++output;
        }
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_difference(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output) {
    while(first != last && first_x != last_x) {
        if(*first < *first_x) {
            *output = *first;
            ++first;
            ++output;
        } else if(*first_x < *first) {
            ++first_x;
        } else {
            ++first;
            ++first_x;
        }
    }
    while(first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOps>
OutputIterator set_difference(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output, BinaryOps op) {
    while(first != last && first_x != last_x) {
        if(op(*first, *first_x)) {
            *output = *first;
            ++first;
            ++output;
        } else if(op(*first_x, *first)) {
            ++first_x;
        } else {
            ++first;
            ++first_x;
        }
    }
    while(first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output) {
    while(first != last && first_x != last_x) {
        if(*first < *first_x) {
            *output = *first;
            ++first;
            ++output;
        } else if(*first_x < *first) {
            *output = *first_x;
            ++first_x;
            ++output;
        } else {
            ++first;
            ++first_x;
        }
    }
    while(first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    while(first_x != last_x) {
        *output = *first_x;
        ++first_x;
        ++output;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOps>
OutputIterator set_symmetric_difference(InputIterator1 first, InputIterator1 last, InputIterator2 first_x, InputIterator2 last_x, OutputIterator output, BinaryOps op) {
    while(first != last && first_x != last_x) {
        if(op(*first, *first_x)) {
            *output = *first;
            ++first;
            ++output;
        } else if(op(*first_x < *first)) {
            *output = *first_x;
            ++first_x;
            ++output;
        } else {
            ++first;
            ++first_x;
        }
    }
    while(first != last) {
        *output = *first;
        ++first;
        ++output;
    }
    while(first_x != last_x) {
        *output = *first_x;
        ++first_x;
        ++output;
    }
    return output;
}

template <class InputIterator>
InputIterator adjacent_find(InputIterator first, InputIterator last) {
    if(first != last) {
        InputIterator next = first;
        ++next;
        while(next != last) {
            if(*first == *next) {
                return first;
            }
            ++first;
            ++next;
        }
    }
    return last;
}

template <class InputIterator, class BinaryOps>
InputIterator adjacent_find(InputIterator first, InputIterator last, BinaryOps op) {
    if(first != last) {
        InputIterator next = first;
        ++next;
        while(next != last) {
            if(op(*first, *next)) {
                return first;
            }
            ++first;
            ++next;
        }
    }
    return last;
}

template <class InputIterator, class T>
size_t count(InputIterator first, InputIterator last, const T &x) {
    size_t result = 0;
    while(first != last) {
        if(*first == x) {
            ++result;
        }
        ++first;
    }
    return result;
}

template <class InputIterator, class UnaryOps>
size_t count_if(InputIterator first, InputIterator last, UnaryOps op) {
    size_t result = 0;
    while(first != last) {
        if(op(*first)) {
            ++result;
        }
        ++first;
    }
    return result;
}

template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T &x) {
    while(first != last) {
        if(*first == x) {
            return first;
        }
        ++first;
    }
    return last;
}

template < class InputIterator, class UnaryOps>
InputIterator find_if(InputIterator first, InputIterator last, UnaryOps op) {
    while(first != last) {
        if(op(*first)) {
            return first;
        }
        ++first;
    }
    return last;
}

template <class InputIterator1, class InputIterator2>
InputIterator1 __find_end(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end, bidirectional_iterator_tag, bidirectional_iterator_tag) {
    typedef hh_std::reverse_iterator<InputIterator1> rit1;
    typedef hh_std::reverse_iterator<InputIterator2> rit2;
    rit1 rfirst(last), rlast(first);
    rit2 rbegin(end), rend(begin);
    return __find_end_aux(rfirst, rlast, rbegin, rend).base();
}

template <class InputIterator1, class InputIterator2>
InputIterator1 __find_end_aux(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end) {
    InputIterator1 pre = last, tmp, tmp2;
    while(first != last) {
        if(*first == *begin) {
            tmp = first;
            tmp2 = begin;
            while(tmp != last && tmp2 != end && *tmp == *tmp2) {
                ++tmp;
                ++tmp2;
            }
            if(tmp2 == end) {
                return tmp;
            }
        }
        ++first;
    }
    return last;
}

template <class InputIterator1, class InputIterator2>
InputIterator1 __find_end(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end, forward_iterator_tag, forward_iterator_tag) {
    InputIterator1 result = last, tmp;
    while(first != last) {
        tmp = search(first, last, begin, end);
        if(tmp == last) {
            return result;
        }
        result = tmp;
        first = result;
        ++first;
    }
    return result;
}

template <class InputIterator1, class InputIterator2>
InputIterator1 find_end(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end) {
    typedef typename iterator_traits<InputIterator1>::iterator_category type1;
    typedef typename iterator_traits<InputIterator2>::iterator_category type2;
    return __find_end(first, last, begin, end, type1(), type2());
    InputIterator1 pre = last, tmp, tmp2;
    while(first != last) {
        if(*first == *begin) {
            tmp = first;
            tmp2 = begin;
            while(tmp != last && tmp2 != end && *tmp == *tmp2) {
                ++tmp;
                ++tmp2;
            }
            if(tmp2 == end) {
                pre = first;
            }
        }
        ++first;
    }
    return pre;
}

template <class InputIterator1, class InputIterator2>
InputIterator1 find_first_of(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end) {
    InputIterator1 pre = last, tmp, tmp2;
    while(first != last) {
        if(*first == *begin) {
            tmp = first;
            tmp2 = begin;
            while(tmp != last && tmp2 != end && *tmp == *tmp2) {
                ++tmp;
                ++tmp2;
            }
            if(tmp2 == end) {
                return first;
            }
        }
        ++first;
    }
    return last;
}

template <class InputIterator1, class InputIterator2, class BinaryOps>
InputIterator1 find_first_of(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end, BinaryOps op) {
    InputIterator1 pre = last, tmp, tmp2;
    while(first != last) {
        if(op(*first, *begin)) {
            tmp = first;
            tmp2 = begin;
            while(tmp != last && tmp2 != end && op(*tmp, *tmp2)) {
                ++tmp;
                ++tmp2;
            }
            if(tmp2 == end) {
                return first;
            }
        }
        ++first;
    }
    return last;
}

template <class InputIterator, class UnaryOps>
void for_each(InputIterator first, InputIterator last, UnaryOps op) {
    while(first != last) {
        op(*first);
        ++first;
    }
}

template <class InputIterator, class Ops>
void generate(InputIterator first, InputIterator last, Ops op) {
    while(first != last) {
        *first = op();
        ++first;
    }
}

template <class InputIterator, class Integer, class Ops>
void generate_n(InputIterator first, Integer n, Ops op) {
    while(n > 0) {
        *first = op();
        ++first;
        --n;
    }
}

template <class InputIterator, class T>
InputIterator remove(InputIterator first, InputIterator last, const T &x) {
    InputIterator anchor = first;
    while(first != last && *first != x) {
        ++first;
    }
    anchor = first;
    while(first != last) {
        if(*first != x) {
            *anchor = *first;
            ++anchor;
        }
        ++first;
    }
    return anchor;
}

template <class InputIterator1, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator1 first, InputIterator1 last, OutputIterator output, const T &x) {
    while(first != last) {
        if(*first != x) {
            *output = x;
            ++output;
        }
        ++first;
    }
    return output;
}

template <class InputIterator, class UnaryOps>
InputIterator remove_if(InputIterator first, InputIterator last, UnaryOps op) {
    InputIterator anchor = first;
    while(first != last && !op(first)) {
        ++first;
    }
    anchor = first;
    while(first != last) {
        if(!op(*first)) {
            *anchor = *first;
            ++anchor;
        }
        ++first;
    }
    return anchor;
}

template <class InputIterator, class OutputIterator, class UnaryOps>
OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator output, UnaryOps op) {
    while(first != last) {
        if(!op(*first)) {
            *output = *first;
            ++output;
        }
        ++first;
    }
    return output;
}

template <class InputIterator, class T>
void replace(InputIterator first, InputIterator last, const T &x, const T &y) {
    while(first != last) {
        if(*first == x) {
            *first = y;
        }
        ++first;
    }
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last, OutputIterator output, const T &x, const T &y) {
    while(first != last) {
        if(*first == x) {
            *output = y;
        } else {
            *output = *first;
        }
        ++first;
        ++output;
    }
    return output;
}

template <class InputIterator, class T, class UnaryOps>
void replace_if(InputIterator first, InputIterator last, UnaryOps op, const T &x) {
    while(first != last) {
        if(op(*first)) {
            *first = x;
        }
        ++first;
    }
}

template <class InputIterator, class OutputIterator, class UnaryOps, class T>
OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator output, UnaryOps op, const T &x) {
    while(first != last) {
        if(op(*first)) {
            *output = x;
        } else {
            *output = *first;
        }
        ++output;
        ++first;
    }
    return output;
}

template <class InputIterator>
void __reverse(InputIterator first, InputIterator last, bidirectional_iterator_tag) {
    while(true) {
        if(first == last || first == --last) {
            return;
        }
        iter_swap(first, last);
        ++first;
    }
}

template <class InputIterator>
void __reverse(InputIterator first, InputIterator last, random_access_iterator_tag) {
    while(first < last) {
        iter_swap(first++, --last);
    }
}

template <class InputIterator>
void reverse(InputIterator first, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __reverse(first, last, type());
}

template <class InputIterator, class OutputIterator>
OutputIterator reverse_copy(InputIterator first, InputIterator last, OutputIterator output) {
    while(first != last) {
        --last;
        *output = *last;
        ++output;
    }
    return output;
}

template <class InputIterator>
void __rotate(InputIterator first, InputIterator middle, InputIterator last, forward_iterator_tag) {
    if(first != last) {
        InputIterator cur = middle;
        while(true) {
            while(first != middle && cur != last) {
                iter_swap(first, cur);
                ++first;
                ++cur;
            }
            if(first == middle) {
                if(cur == last) return;
                middle = cur;
            } else {
                cur = middle;
            }
        }
    }
}

template <class InputIterator>
void __rotate(InputIterator first, InputIterator middle, InputIterator last, bidirectional_iterator_tag) {
    hh_std::reverse(first, middle);
    hh_std::reverse(middle, last);
    hh_std::reverse(first, last);
}

//template <class InputIterator>
//void __rotate(InputIterator first, InputIterator middle, InputIterator last, random_access_iterator_tag) {
//
//}

template<class InputIterator>
void rotate(InputIterator first, InputIterator middle, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __rotate(first, middle, last, type());
}

template<class InputIterator, class OutputIterator>
OutputIterator rotate_copy(InputIterator first, InputIterator middle, InputIterator last, OutputIterator output) {
    InputIterator tmp = middle;
    while(tmp != last) {
        *output = *tmp;
        ++output;
        ++tmp;
    }
    while(first != middle) {
        *output = *first;
        ++output;
        ++first;
    }
    return output;
}

template <class InputIterator1, class InputIterator2>
InputIterator1 search(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end) {
    return hh_std::find_first_of(first, last, begin, end);
}

template <class InputIterator, class Integer, class T>
InputIterator search_n(InputIterator first, InputIterator last, Integer n, const T &x) {
    while(first != last) {
        if(*first == x) {
            Integer count = 0;
            InputIterator cur = first;
            while(cur != last && *cur == x && count < n) {
                ++cur;
                ++count;
            }
            if(count == n) {
                return first;
            }
            first = cur;
            continue;
        }
        ++first;
    }
    return last;
}

template <class InputIterator,  class Integer, class T, class BinaryOps>
InputIterator search_n(InputIterator first, InputIterator last, Integer n, const T &x, BinaryOps op) {
    while(first != last) {
        if(op(*first, x)) {
            Integer count = 0;
            InputIterator cur = first;
            while(cur != last && op(*cur, x) && count < n) {
                ++cur;
                ++count;
            }
            if(count == n) {
                return first;
            }
            first = cur;
            continue;
        }
        ++first;
    }
    return last;
}

template <class InputIterator1, class InputIterator2>
InputIterator2 swap_ranges(InputIterator1 first, InputIterator1 last, InputIterator2 begin) {
    while(first != last) {
        hh_std::iter_swap(first, begin);
        ++first;
        ++begin;
    }
    return begin;
}

template <class InputIterator, class OutputIterator, class UnaryOps>
OutputIterator transform(InputIterator first, InputIterator last, OutputIterator output, UnaryOps op) {
    while(first != last) {
        *output = op(*first);
        ++first;
        ++output;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class BinaryOps>
OutputIterator transform(InputIterator1 first, InputIterator1 last, InputIterator2 begin, OutputIterator output, BinaryOps op) {
    while(first != last) {
        *output = op(*first, *begin);
        ++first;
        ++begin;
        ++output;
    }
    return output;
}

template <class InputIterator>
InputIterator max_element(InputIterator first, InputIterator last) {
    InputIterator result = first;
    while(first != last) {
        if(*first > *result) {
            result = first;
        }
        ++first;
    }
    return result;
}

template <class InputIterator, class Compare>
InputIterator max_element(InputIterator first, InputIterator last, Compare comp) {
    InputIterator result = first;
    while(first != last) {
        if(comp(*result, *first)) {
            result = first;
        }
        ++first;
    }
    return result;
}

template <class InputIterator>
InputIterator min_element(InputIterator first, InputIterator last) {
    InputIterator result = first;
    while(first != last) {
        if(*first < *result) {
            result = first;
        }
        ++first;
    }
    return result;
}

template <class InputIterator, class Compare>
InputIterator min_element(InputIterator first, InputIterator last, Compare comp) {
    InputIterator result = first;
    while(first != last) {
        if(comp(*first, *result)) {
            result = first;
        }
        ++first;
    }
    return result;
}

template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end) {
    while(first != last && begin != end) {
        if(*first < *begin) {
            ++first;
        } else if(*begin < *first) {
            return false;
        } else {
            ++first;
            ++begin;
        }
    }
    return begin == end;
}

template <class InputIterator1, class InputIterator2, class BinaryOps>
bool includes(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end, BinaryOps op) {
    while(first != last && begin != end) {
        if(op(*first, *begin)) {
            ++first;
        } else if(op(*begin, *first)) {
            return false;
        } else {
            ++first;
            ++begin;
        }
    }
    return begin == end;
}

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end, OutputIterator output) {
    while(first != last && begin != end) {
        if(*first < *begin) {
            *output = *first;
            ++first;
        } else {
            *output = *begin;
            ++begin;
        }
        ++output;
    }
    return output;
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator merge(InputIterator1 first, InputIterator1 last, InputIterator2 begin, InputIterator2 end, OutputIterator output, Compare comp) {
    while(first != last && begin != end) {
        if(comp(*first, *begin)) {
            *output = *first;
            ++first;
        } else {
            *output = *begin;
            ++begin;
        }
        ++output;
    }
    return output;
}

template <class InputIterator, class UnaryOps>
InputIterator partition(InputIterator first, InputIterator last, UnaryOps op) {
    InputIterator result = first;
    while(first != last && op(*first)) {
        ++first;
    }
    result = first;
    while(first != last) {
        if(op(*first)) {
            iter_swap(result, first);
            ++result;
        }
        ++first;
    }
    return result;
}

template <class InputIterator>
InputIterator unique(InputIterator first, InputIterator last) {
    InputIterator anchor = first, tmp = first;
    if(first != last) {
        ++first;
        ++anchor;
        while(first != last) {
            if(*first != *tmp) {
                if(first != anchor) {
                    *anchor = *first;
                }
                ++anchor;
            }
            ++first;
            ++tmp;
        }
    }
    return anchor;
}

template <class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last, OutputIterator output) {
    if(first != last) {
        *output = first;
        ++output;
        InputIterator tmp = first;
        ++first;
        while(first != last) {
            if(*first != *tmp) {
                *output = *first;
                ++output;
            }
            ++first;
            ++tmp;
        }
    }
    return output;
}

template <class InputIterator, class T>
InputIterator __lower_bound(InputIterator first, InputIterator last, const T & x, forward_iterator_tag) {
    while(first != last) {
        if(*first >= x) {
            return first;
        }
        ++first;
    }
    return first;
}

template <class InputIterator, class T>
InputIterator __lower_bound(InputIterator first, InputIterator last, const T & x, const random_access_iterator_tag & tag) {
    if(first != last) {
        InputIterator middle = first + (last - first) / 2;
        if(*middle < x) {
            return __lower_bound(middle + 1, last, x, tag);
        } else {
            return __lower_bound(first, middle, x, tag);
        }
    }
    return last;
}

template <class InputIterator, class T>
InputIterator lower_bound(InputIterator first, InputIterator last, const T & x) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    return __lower_bound(first, last, x, type());
}

//template <class InputIterator, class UnaryOps>
//InputIterator __lower_bound(InputIterator first, InputIterator last, UnaryOps op, forward_iterator_tag) {
//    while(first != last) {
//        if(*first >= x) {
//            return first;
//        }
//        ++first;
//    }
//    return first;
//}
//
//template <class InputIterator, class UnaryOps>
//InputIterator __lower_bound(InputIterator first, InputIterator last, UnaryOps op, const random_access_iterator_tag &tag) {
//    if(first != last) {
//        InputIterator middle = first + (last - first) / 2;
//        if(*middle < x) {
//            return __lower_bound(middle + 1, last, x, tag);
//        } else {
//            return __lower_bound(first, middle, x, tag);
//        }
//    }
//    return last;
//}

//template <class InputIterator, class UnaryOps>
//InputIterator lower_bound(InputIterator first, InputIterator last, UnaryOps op) {
//    while(first != last) {
//        if(op(*first)) {
//            return first;
//        }
//        ++first;
//    }
//    return first;
//}

template <class InputIterator, class T>
InputIterator __upper_bound(InputIterator first, InputIterator last, const T & x, forward_iterator_tag) {
    while(first != last) {
        if(*first > x) {
            return first;
        }
        ++first;
    }
    return first;
}

template <class InputIterator, class T>
InputIterator __upper_bound(InputIterator first, InputIterator last, const T & x, const random_access_iterator_tag & tag) {
    if(first != last) {
        InputIterator middle = first + (last - first) / 2;
        if(x < *middle) {
            return __upper_bound(first, middle, x, tag);
        } else {
            return __upper_bound(middle + 1, last, x, tag);
        }
    }
    return last;
}

template <class InputIterator, class T>
InputIterator upper_bound(InputIterator first, InputIterator last, const T & x) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    return __upper_bound(first, last, x, type());
}

//template <class InputIterator, class UnaryOps>
//InputIterator upper_bound(InputIterator first, InputIterator last, UnaryOps op) {
//    while(first != last) {
//        if(op(*first)) {
//            return first;
//        }
//        ++first;
//    }
//    return first;
//}

template <class InputIterator, class T>
bool __binary_search(InputIterator first, InputIterator last, const T & x, forward_iterator_tag) {
    while(first != last) {
        if(*first == x) {
            return true;
        }
        ++first;
    }
    return false;
}

template <class InputIterator, class T>
bool __binary_search(InputIterator first, InputIterator last, const T & x, random_access_iterator_tag) {
    --last;
    ptrdiff_t len = last - first;
    size_t left = 0, right = len;
    while(left <= right) {
        size_t middle = (left + right) / 2;
        if(*(first + middle) == x) {
            return true;
        } else if(*(first + middle) < x) {
            left = middle + 1;
        } else {
            right = middle - 1;
        }
    }
    return false;
}

template <class InputIterator, class T>
bool binary_search(InputIterator first, InputIterator last, const T & x) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    return __binary_search(first, last, x, type());
}

template <class InputIterator>
bool next_permutation(InputIterator first, InputIterator last) {
    if(first == last || first == --last) {
        return false;
    }
    typedef typename iterator_traits<InputIterator>::value_type type;
    InputIterator max_element = last;
    InputIterator cur = last;
    --cur;
    while(true) {
        if(*cur < *max_element) {
            InputIterator tmp = cur;
            ++tmp;
            while(true) {
                if(*tmp > *cur && *tmp < *max_element) {
                    max_element = tmp;
                }
                if(tmp == last) {
                    iter_swap(cur, max_element);
                    return true;
                }
                ++tmp;
            }
        }
        if(*cur > *max_element) {
            max_element = cur;
        }
        if(cur == first) return false;
        --cur;
    }
}

template <class InputIterator, class BinaryOps>
bool next_permutation(InputIterator first, InputIterator last, BinaryOps op) {
    if(first == last || first == --last) {
        return false;
    }
    typedef typename iterator_traits<InputIterator>::value_type type;
    InputIterator max_element = last;
    InputIterator cur = last;
    --cur;
    while(true) {
        if(op(*cur, *max_element)) {
            InputIterator tmp = cur;
            ++tmp;
            while(true) {
                if(op(*cur, *tmp) && op(*tmp, *max_element)) {
                    max_element = tmp;
                }
                if(tmp == last) {
                    iter_swap(cur, max_element);
                    return true;
                }
                ++tmp;
            }
        }
        if(op(*max_element, *cur)) {
            max_element = cur;
        }
        if(cur == first) return false;
        --cur;
    }
}

template <class InputIterator>
bool prev_permutation(InputIterator first, InputIterator last) {
    if(first == last || first == --last) {
        return false;
    }
    typedef typename iterator_traits<InputIterator>::value_type type;
    InputIterator min_element = last;
    InputIterator cur = last;
    --cur;
    while(true) {
        if(*cur > *min_element) {
            InputIterator tmp = cur;
            ++tmp;
            while(true) {
                if(*tmp < *cur && *tmp > *min_element) {
                    min_element = tmp;
                }
                if(tmp == last) {
                    iter_swap(cur, min_element);
                    return true;
                }
                ++tmp;
            }
        }
        if(*cur < *min_element) {
            min_element = cur;
        }
        if(cur == first) return false;
        --cur;
    }
}

template <class InputIterator, class BinaryOps>
bool prev_permutation(InputIterator first, InputIterator last, BinaryOps op) {
    if(first == last || first == --last) {
        return false;
    }
    typedef typename iterator_traits<InputIterator>::value_type type;
    InputIterator min_element = last;
    InputIterator cur = last;
    --cur;
    while(true) {
        if(op(*min_element, *cur)) {
            InputIterator tmp = cur;
            ++tmp;
            while(true) {
                if(op(*tmp, *cur) && op(*min_element, *tmp)) {
                    max_element = tmp;
                }
                if(tmp == last) {
                    iter_swap(cur, max_element);
                    return true;
                }
                ++tmp;
            }
        }
        if(op(*cur, *min_element)) {
            min_element = cur;
        }
        if(cur == first) return false;
        --cur;
    }
}

template <class InputIterator>
void __sort(InputIterator first, InputIterator last, const hh_std::bidirectional_iterator_tag & tag) {
    if(first != last && first != --last) {
        InputIterator left = first, right = last;
        ++left;
        while(left != right && *left < *first) ++left;
        if(left != right) iter_swap(left, right);
        while(left != right && *right >= *first) --right;
        if(left != right) iter_swap(left, right);
        if(*left < *first) {
            iter_swap(left, first);
        } else {
            iter_swap(--left, first);
        }
        __sort(first, left, tag);
        __sort(++left, ++last, tag);
    }
}

template <class InputIterator>
void __sort(InputIterator first, InputIterator last, const hh_std::forward_iterator_tag & tag) {
    if(first != last) {
        InputIterator min_element = first, tmp;
        tmp = first;
        while(tmp != last) {
            if(*tmp < *min_element) {
                min_element = tmp;
            }
            ++tmp;
        }
        if(min_element != first) iter_swap(min_element, first);
        __sort(++first, last, tag);
    }
}

template <class InputIterator>
void sort(InputIterator first, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __sort(first, last, type());
}

template <class InputIterator, class BinaryOps>
void __sort(InputIterator first, InputIterator last, BinaryOps op, const hh_std::bidirectional_iterator_tag & tag) {
    if(first != last && first != --last) {
        InputIterator left = first, right = last;
        ++left;
        while(left != right && op(*left, *first)) ++left;
        if(left != right) iter_swap(left, right);
        while(left != right && !op(*right, *first)) --right;
        if(left != right) iter_swap(left, right);
        if(op(*left, *first)) {
            iter_swap(left, first);
        } else {
            iter_swap(--left, first);
        }
        __sort(first, left, tag);
        __sort(++left, ++last, tag);
    }
}

template <class InputIterator, class BinaryOps>
void __sort(InputIterator first, InputIterator last, BinaryOps op, const hh_std::forward_iterator_tag & tag) {
    if(first != last) {
        InputIterator min_element = first, tmp;
        tmp = first;
        while(tmp != last) {
            if(op(*tmp, *min_element)) {
                min_element = tmp;
            }
            ++tmp;
        }
        if(min_element != first) iter_swap(min_element, first);
        __sort(++first, last, tag);
    }
}

template <class InputIterator, class BinaryOps>
void sort(InputIterator first, InputIterator last, BinaryOps op) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __sort(first, last, op, type());
}

template <class InputIterator>
void __partial_sort(InputIterator first, InputIterator middle, InputIterator last, random_access_iterator_tag) {
    if(first != middle && first != last) {
        hh_std::make_heap(first, middle);
        for(InputIterator cur = middle; cur != last; ++cur) {
            if(*cur < *first) {
                hh_std::pop_heap(first, middle);
                iter_swap(cur, --middle);
                ++middle;
                hh_std::push_heap(first, middle);
            }
        }
        hh_std::sort_heap(first, middle);
    }
}

template <class InputIterator>
void __partial_sort(InputIterator first, InputIterator middle, InputIterator last, const forward_iterator_tag &tag) {
    if(first != last && first != middle) {
        InputIterator min_element = first, tmp = first, next;
        ++tmp;
        while(tmp != last) {
            if(*tmp < *min_element) {
                min_element = tmp;
            }
            ++tmp;
        }
        if(first != min_element) {
            tmp = first;
            next = tmp;
            ++next;
            while(true) {
                iter_swap(tmp, next);
                if(next == min_element) {
                    iter_swap(first, tmp);
                    break;
                }
                ++next;
            }
        }
        __partial_sort(++first, middle, last, tag);
    }
}

template <class InputIterator>
void partial_sort(InputIterator first, InputIterator middle, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __partial_sort(first, middle, last, type());
}

template <class InputIterator>
void __random_shuffle(InputIterator first, InputIterator last, forward_iterator_tag) {
    typedef typename iterator_traits<InputIterator>::difference_type type;
    type len = hh_std::distance(first, last);
    InputIterator tmp;
    if(len > 1) {
        while(first != last) {
            size_t pos = std::rand() % len;
            tmp = first;
            hh_std::advance(tmp, pos);
            iter_swap(first, tmp);
            ++first;
            --len;
        }
    }
}

template <class InputIterator>
void __random_shuffle(InputIterator first, InputIterator last, random_access_iterator_tag) {
    typedef typename iterator_traits<InputIterator>::difference_type type;
    type len = last - first;
    InputIterator tmp;
    if(len > 1) {
        while(first != last) {
            size_t pos = std::rand() % len;
            tmp = first + pos;
            iter_swap(first, tmp);
            ++first;
            --len;
        }
    }
}

template <class InputIterator>
void random_shuffle(InputIterator first, InputIterator last) {
    typedef typename hh_std::iterator_traits<InputIterator>::iterator_category type;
    __random_shuffle(first, last, type());
}

template <class InputIterator>
void __stable_sort_aux(InputIterator first, InputIterator last, size_t len, const forward_iterator_tag & tag) {
    if(len > 1) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        InputIterator middle = first;
        advance(middle, len / 2);
        __stable_sort_aux(first, middle, len / 2, tag);
        __stable_sort_aux(middle, last, (len + 1) / 2, tag);
        size_t pos = 0;
        type *data = new type[len];
        InputIterator left = first, right = middle;
        while(pos < len && left != middle && right != last) {
            if(*right < *left) {
                data[pos++] = *right;
                ++right;
            } else {
                data[pos++] = *left;
                ++left;
            }
        }
        while(left != middle) {
            data[pos++] = *(left++);
        }
        while(right != last) {
            data[pos++] = *(right++);
        }
        pos = 0;
        while(pos < len && first != last) {
            *(first++) = data[pos++];
        }
        delete [] data;
    }
}

template <class InputIterator>
void __stable_sort(InputIterator first, InputIterator last, const forward_iterator_tag & tag) {
    size_t len = distance(first, last);
    __stable_sort_aux(first, last, len, tag);
}

template <class InputIterator>
void __stable_sort(InputIterator first, InputIterator last, const random_access_iterator_tag & tag) {
    if(first != last && first != last - 1) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        InputIterator middle = first + (last - first) / 2;
        __stable_sort(first, middle, tag);
        __stable_sort(middle, last, tag);
        size_t pos = 0, len = last - first;
        type *data = new type[len];
        InputIterator left = first, right = middle;
        while(pos < len && left != middle && right != last) {
            if(*right < *left) {
                data[pos++] = *right;
                ++right;
            } else {
                data[pos++] = *left;
                ++left;
            }
        }
        while(left != middle) {
            data[pos++] = *(left++);
        }
        while(right != last) {
            data[pos++] = *(right++);
        }
        while(pos > 0 && first != last) {
            *(--last) = data[--pos];
        }
        delete [] data;
    }
}

template <class InputIterator>
void stable_sort(InputIterator first, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __stable_sort(first, last, type());
}

template <class InputIterator, class BinaryOps>
void __stable_sort_aux(InputIterator first, InputIterator last, size_t len, BinaryOps op, const forward_iterator_tag & tag) {
    if(len > 1) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        InputIterator middle = first;
        advance(middle, len / 2);
        __stable_sort_aux(first, middle, len / 2, tag);
        __stable_sort_aux(middle, last, (len + 1) / 2, tag);
        size_t pos = 0;
        type *data = new type[len];
        InputIterator left = first, right = middle;
        while(pos < len && left != middle && right != last) {
            if(op(*right, *left)) {
                data[pos++] = *right;
                ++right;
            } else {
                data[pos++] = *left;
                ++left;
            }
        }
        while(left != middle) {
            data[pos++] = *(left++);
        }
        while(right != last) {
            data[pos++] = *(right++);
        }
        pos = 0;
        while(pos < len && first != last) {
            *(first++) = data[pos++];
        }
        delete [] data;
    }
}

template <class InputIterator, class BinaryOps>
void __stable_sort(InputIterator first, InputIterator last, BinaryOps op, const forward_iterator_tag & tag) {
    size_t len = distance(first, last);
    __stable_sort_aux(first, last, len, op, tag);
}

template <class InputIterator, class BinaryOps>
void __stable_sort(InputIterator first, InputIterator last, BinaryOps op, const random_access_iterator_tag & tag) {
    if(first != last && first != last - 1) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        InputIterator middle = first + (last - first) / 2;
        __stable_sort(first, middle, tag);
        __stable_sort(middle, last, tag);
        size_t pos = 0, len = last - first;
        type *data = new type[len];
        InputIterator left = first, right = middle;
        while(pos < len && left != middle && right != last) {
            if(op(*right, *left)) {
                data[pos++] = *right;
                ++right;
            } else {
                data[pos++] = *left;
                ++left;
            }
        }
        while(left != middle) {
            data[pos++] = *(left++);
        }
        while(right != last) {
            data[pos++] = *(right++);
        }
        while(pos > 0 && first != last) {
            *(--last) = data[--pos];
        }
        delete [] data;
    }
}

template <class InputIterator, class BinaryOps>
void stable_sort(InputIterator first, InputIterator last, BinaryOps op) {
    typedef typename iterator_traits<InputIterator>::iterator_category type;
    __stable_sort(first, last, op, type());
}

template <class InputIterator, class T>
hh_std::pair<InputIterator, InputIterator> equal_range(InputIterator first, InputIterator last, const T & x) {
    InputIterator left = lower_bound(first, last, x);
    if(left == last || x < *left) {
        return hh_std::make_pair(left, left);
    }
    InputIterator right = upper_bound(left, last, x);
    return hh_std::make_pair(left, right);
}

template <class InputIterator>
void nth_element(InputIterator first, InputIterator target, InputIterator last) {
    bool flag = false;
    if(first != last) {
        InputIterator left = first;
        while(first != last) {
            if(first == target) {
                ++first;
            } else if(*first < *target) {
                if(first != left) {
                    iter_swap(first, left);
                }
                ++first;
                ++left;
            } else {
                ++first;
            }
            if(left == target) {
                ++left;
                flag = true;
            }
        }
        if(flag) {
            --left;
        }
        iter_swap(target, left);
    }
}

template <class InputIterator, class BinaryOps>
void nth_element(InputIterator first, InputIterator target, InputIterator last, BinaryOps op) {
    bool flag = false;
    if(first != last) {
        InputIterator left = first;
        while(first != last) {
            if(first == target) {
                ++first;
            } else if(op(*first, *target)) {
                if(first != left) {
                    iter_swap(first, left);
                }
                ++first;
                ++left;
            } else {
                ++first;
            }
            if(left == target) {
                ++left;
                flag = true;
            }
        }
        if(flag) {
            --left;
        }
        iter_swap(target, left);
    }
}

//template <class InputIterator, class UnaryOps>
//void __stable_partition(InputIterator first, InputIterator last, UnaryOps op, const random_access_iterator_tag &tag) {
//}
//
//template <class InputIterator, class UnaryOps>
//void __stable_partition(InputIterator first, InputIterator last, UnaryOps op, const forward_iterator_tag &tag) {
//
//}

template <class InputIterator, class UnaryOps>
void stable_partition(InputIterator first, InputIterator last, UnaryOps op) {
    if(first != last) {
        typedef typename iterator_traits<InputIterator>::value_type type;
        while(first != last && op(*first)) ++first;
        InputIterator cur = first;
        type tmp;
        while(cur != last && !op(*cur)) ++cur;
        while(cur != last) {
            tmp = *cur;
            hh_std::copy_backward(first, cur, cur + 1);
            *first = tmp;
            ++first;
            while(cur != last && !op(*cur)) ++cur;
        }
    }
    //typedef typename iterator_traits<InputIterator>::iterator_category type;
    //__stable_partition(first, last, op, type());
}

template <class InputIterator>
void inplace_merge(InputIterator first, InputIterator middle, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::value_type type;
    typedef typename iterator_traits<InputIterator>::difference_type size;
    size len1 = distance(first, middle), len2 = distance(middle, last), pos = 0;
    type *data = new type[len1 + len2];
    InputIterator left = first, right = middle;
    while(left != middle && right != last) {
        if(*right < *left) {
            data[pos++] = *right;
            ++right;
        } else {
            data[pos++] = *left;
            ++left;
        }
    }
    while(left != middle) {
        data[pos++] = *(left++);
    }
    while(right != last) {
        data[pos++] = *(right++);
    }
    hh_std::copy(data, data + pos, first);
    delete [] data;
}

template <class InputIterator, class Compare>
void inplace_merge(InputIterator first, InputIterator middle, InputIterator last, Compare comp) {
    typedef typename iterator_traits<InputIterator>::value_type type;
    typedef typename iterator_traits<InputIterator>::difference_type size;
    size len1 = distance(first, middle), len2 = distance(middle, last), pos = 0;
    type *data = new type[len1 + len2];
    InputIterator left = first, right = middle;
    while(left != middle && right != last) {
        if(comp(*right, *left)) {
            data[pos++] = *right;
            ++right;
        } else {
            data[pos++] = *left;
            ++left;
        }
    }
    while(left != middle) {
        data[pos++] = *(left++);
    }
    while(right != last) {
        data[pos++] = *(right++);
    }
    hh_std::copy(data, data + pos, first);
    delete [] data;
}

};

#endif
