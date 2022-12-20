#ifndef __HH_STL_HASH_FUN_H_
#define __HH_STL_HASH_FUN_H_

#include<cstdlib>

namespace hh_std {

inline size_t __hh_stl_hash_string(const char* s) {
    size_t result = 0;
    for(; *s; ++s) {
        result = 5 * result + *s;
    }
    return result;
}

inline size_t __hh_stl_hash(const char* s, size_t len) {
    size_t result = 0, pos = 0;
    for(; pos < len; ++s, ++pos) {
        result = 5 * result + *s;
    }
    return result;
}

template <class T>
struct hash {
    size_t operator() (T val) const {
        return __hh_stl_hash((char*)&val, sizeof(val));
    }
};

template <>
struct hash<char*> {
    size_t operator() (const char* s) const {
        return __hh_stl_hash_string(s);
    }
};

template <>
struct hash<const char*> {
    size_t operator() (const char* s) const {
        return __hh_stl_hash_string(s);
    }
};

template <>
struct hash<char> {
    size_t operator() (char c) const {
        return c;
    }
};

template <>
struct hash<signed char> {
    size_t operator() (signed char c) const {
        return c;
    }
};

template <>
struct hash<unsigned char> {
    size_t operator() (unsigned char c) const {
        return c;
    }
};

template <>
struct hash<short> {
    size_t operator() (short s) const {
        return s;
    }
};

template <>
struct hash<unsigned short> {
    size_t operator() (unsigned short s) const {
        return s;
    }
};

template <>
struct hash<int> {
    size_t operator() (int s) const {
        return s;
    }
};

template <>
struct hash<unsigned int> {
    size_t operator() (unsigned int s) const {
        return s;
    }
};

template <>
struct hash<long> {
    size_t operator() (long s) const {
        return s;
    }
};

template <>
struct hash<unsigned long> {
    size_t operator() (unsigned long s) const {
        return s;
    }
};


};

#endif
