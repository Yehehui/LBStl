#ifndef __HH_STL_ALLOC_H_
#define __HH_STL_ALLOC_H_

#if 0
#   include <new>
#   define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)
#   include <iostream>
#   define __THROW_BAD_ALLOC std::cerr<<"out of memory"<<std::endl;exit(1);
#endif

#include <cstdio>
#include <cstddef>
#include <cstring>

namespace hh_std {

template <int inst>
class __malloc_alloc_template {
private:
    static void *oom_alloc(size_t);
    static void *oom_realloc(void*, size_t);
    static void  (*__malloc_alloc_oom_handler)();
public:
    static void* allocate(size_t n) {
        void *result = malloc(n);
        if(result == 0) result = oom_alloc(n);
        return result;
    }

    static void* reallocate(void *p, size_t n, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if(result == 0) result = oom_realloc(p, new_sz);
        return result;
    }

    static void deallocate(void *p, size_t n) {
        free(p);
    }

    static void (*set_oom_handler(void (*f)()))() {
        void (*old)() = __malloc_alloc_oom_handler;
        __malloc_alloc_oom_handler = f;
        return old;
    }
};

template <int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = 0;

template <int inst>
void * __malloc_alloc_template<inst>::oom_alloc(size_t n) {
    void (*my_handler)();
    void *result;
    while(true) {
        my_handler = __malloc_alloc_oom_handler;
        if (my_handler == 0) __THROW_BAD_ALLOC;
        my_handler();
        result = malloc(n);
        if(result != 0)
            return result;
    }
}

template <int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *p, size_t sz) {
    void (*myhandler)();
    void *result;
    while(true) {
        myhandler  = __malloc_alloc_oom_handler;
        if(myhandler == 0) __THROW_BAD_ALLOC;
        myhandler();
        result = realloc(p, sz);
        if(result != 0)
            return result;
    }
}

typedef __malloc_alloc_template<0> malloc_alloc;

enum {__ALIGN = 8};
enum {__MAX_BYTES = 128};
enum {__NFREELISTS = __MAX_BYTES / __ALIGN};

template <int inst>
class __default_alloc_template {
private:
    union obj {
        obj *next;
        char data[1];
    };

    static size_t ROUND_UP(size_t n) {
        return (n + __ALIGN - 1) / __ALIGN * __ALIGN;
    }

    static size_t FREELIST_INDEX(size_t n) {
        return (n - 1) / __ALIGN;
    }

    static obj* volatile free_list[__NFREELISTS];
    static void *refill(size_t);
    static char *chunk_alloc(size_t n, int &objs);
    static char *start_free;
    static char *end_free;
    static size_t heap_size;

public:
    static void *allocate(size_t n);
    static void *reallocate(void *p, size_t n, size_t sz);
    static void deallocate(void *p, size_t n);
};

template <int inst>
char *__default_alloc_template<inst>::start_free = 0;
template <int inst>
char *__default_alloc_template<inst>::end_free = 0;
template <int inst>
size_t __default_alloc_template<inst>::heap_size = 0;
template <int inst>
typename __default_alloc_template<inst>::obj* volatile __default_alloc_template<inst>::free_list[__NFREELISTS] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template <int inst>
void *__default_alloc_template<inst>::allocate(size_t n) {
    if(n > __MAX_BYTES) {
        return malloc_alloc::allocate(n);
    }
    size_t index = FREELIST_INDEX(n);
    void *result;
    if (free_list[index]) {
        result = free_list[index];
        free_list[index] = free_list[index]->next;
    } else {
        result = refill(ROUND_UP(n));
    }
    return result;
}

template <int inst>
void *__default_alloc_template<inst>::reallocate(void *p, size_t n, size_t sz) {
    if (sz > __MAX_BYTES) {
        return malloc_alloc::reallocate(p, n, sz);
    }
    if(ROUND_UP(n) == ROUND_UP(sz))
        return p;
    size_t index = FREELIST_INDEX(sz);
    void *result;
    if(free_list[index]) {
        result = free_list[index];
        free_list[index] = free_list[index]->next;
    } else {
        result = refill(ROUND_UP(sz));
    }
    size_t copy_sz = n > sz ? sz : n;
    memmove(result, p, copy_sz);
    return result;
}

template <int inst>
void __default_alloc_template<inst>::deallocate(void *p, size_t n) {
    size_t index = FREELIST_INDEX(n);
    ((obj*)p)->next = free_list[index];
    free_list[index] = (obj*)p;
}

template <int inst>
void *__default_alloc_template<inst>::refill(size_t n) {
    int objs = 20;
    void *result = chunk_alloc(n, objs);
    if (objs > 1) {
        size_t index = FREELIST_INDEX(n);
        obj* cur_data = free_list[index] = (obj*)result + n;
        obj* next_data;
        while(objs > 1) {
            cur_data->next = next_data;
            next_data = (obj*)((char*)cur_data + n);
            objs--;
        }
        cur_data->next = 0;
    }
    return result;
}

template <int inst>
char *__default_alloc_template<inst>::chunk_alloc(size_t n, int &objs) {
    size_t total_require = n * objs;
    size_t total_left = end_free - start_free;
    char *result;
    if (total_left >= total_require) {
        result = start_free;
        start_free += total_require;
        return result;
    } else if(total_left >= n) {
        objs = total_left / n;
        total_require = n * objs;
        result = start_free;
        start_free += total_require;
        return result;
    } else {
        size_t index;
        if (total_left > 0) {
            size_t index = FREELIST_INDEX(total_left);
            ((obj*)start_free)->next = free_list[index];
            free_list[index] = (obj*)start_free;
        }
        size_t malloc_size = 2 * total_require + heap_size;
        result = (char*)malloc(malloc_size);
        if (result == 0) {
            for(index = FREELIST_INDEX(n); index < __NFREELISTS; index++) {
                if(free_list[index] != 0) {
                    start_free = (char*)free_list[index];
                    end_free = start_free + (index + 1) * __ALIGN;
                    free_list[index] = free_list[index]->next;
                    return chunk_alloc(n, objs);
                }
            }
            start_free = end_free = 0;
            result = (char*)malloc_alloc::allocate(malloc_size);
        }
        start_free = result;
        end_free = start_free + malloc_size;
        heap_size = malloc_size;
        return chunk_alloc(n, objs);
    }
}

#ifdef __USE_MALLOC
typedef malloc_alloc alloc;
#else
typedef __default_alloc_template<0> alloc;
#endif

template <class T, class Alloc>
struct simple_alloc {
    static T *allocate(size_t n) {
        return (T*)Alloc::allocate(sizeof(T) * n);
    }
    static T *allocate(void) {
        return (T*)Alloc::allocate(sizeof(T));
    }
    static void deallocate(T *p) {
        Alloc::deallocate(p, sizeof(T));
    }
    static void deallocate(T *p, size_t n) {
        Alloc::deallocate(p, n * sizeof(T));
    }
};

};
#endif
