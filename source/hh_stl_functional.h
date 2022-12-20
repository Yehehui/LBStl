#ifndef __HH_STL_FUNCTIONAL_H_
#define __HH_STL_FUNCTIONAL_H_

#include "hh_stl_numeric.h"

namespace hh_std {

template <class T>
struct identity: public hh_std::unary_function<T, T> {

    T& operator() (const T &value) const {
        return (T&)value;
    }

};

template <class Operation>
struct binder1st: public unary_function <typename Operation::second_argument_type, typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::first_argument_type value;

public:
    binder1st(const Operation &o, const typename Operation::first_argument_type &val): op(o), value(val) {}
    typename Operation::result_type operator() (const typename Operation::second_argument_type &val) {
        return op(value, val);
    }
};

template <class Operation, class T>
binder1st<Operation> bind1st(const Operation &op, const T &x) {
    typedef typename Operation::first_argument_type type;
    return binder1st<Operation>(op, type(x));
}

template <class Operation>
struct binder2nd: public unary_function <typename Operation::first_argument_type, typename Operation::result_type> {
protected:
    Operation op;
    typename Operation::second_argument_type value;

public:
    binder2nd(Operation &o, typename Operation::second_argument_type &val): op(o), value(val) {}
    typename Operation::result_type operator() (const typename Operation::first_argument_type &val) {
        return op(val, value);
    }
};

template <class Operation, class T>
binder1st<Operation> bind2nd(const Operation &op, const T &x) {
    typedef typename Operation::second_argument_type type;
    return binder1st<Operation>(op, type(x));
}

template <class Arg, class Result>
struct pointer_to_unary_function: public unary_function<Arg, Result> {
protected:
    Result (*ptr)(Arg);
public:
    pointer_to_unary_function() {}
    pointer_to_unary_function(const Result (*p)(Arg)): ptr(p) {}

    Result operator() (const Arg &x) const {
        return ptr(x);
    }
};

template <class Arg, class Result>
inline pointer_to_unary_function<Arg, Result> ptr_fun(const Result (*p)(Arg)) {
    return pointer_to_unary_function<Arg, Result>(p);
}

template <class Arg1, class Arg2, class Result>
struct pointer_to_binary_function: public hh_std::binary_function<Arg1, Arg2, Result> {
protected:
    Result (*ptr)(Arg1, Arg2);
public:
    pointer_to_binary_function() {}
    pointer_to_binary_function(const Result (*p)(Arg1, Arg2)): ptr(p) {}

    Result operator() (const Arg1 &x, const Arg2 &y) const {
        return ptr(x, y);
    }
};

template <class Arg1, class Arg2, class Result>
inline pointer_to_binary_function<Arg1, Arg2, Result> ptr_fun(const Result (*p)(Arg1, Arg2)) {
    return pointer_to_binary_function<Arg1, Arg2, Result>(p);
}

template <class Result, class T>
struct mem_fun_t: public unary_function<T*, Result> {
private:
    Result (T::*ptr)();
public:
    explicit mem_fun_t(Result (T::*p)()): ptr(p) {}
    Result operator() (T *x) const {
        return x->*ptr();
    }
};

template <class Result, class T>
inline mem_fun_t<Result, T> mem_fun(Result (T::*p)()) {
    return mem_fun_t<Result, T>(p);
}

template <class Result, class T>
struct const_mem_fun_t: public unary_function<const T*, Result> {
private:
    Result (T::*ptr)() const;
public:
    explicit const_mem_fun_t(Result (T::*p)() const ): ptr(p) {}
    Result operator() (const T *x) const {
        return x->*ptr();
    }
};

template <class Result, class T>
inline const_mem_fun_t<Result, T> mem_fun(Result (T::*p)() const) {
    return const_mem_fun_t<Result, T>(p);
}

template <class Result, class T>
struct mem_fun_ref_t: public unary_function<T, Result> {
private:
    Result (T::*ptr)();
public:
    explicit mem_fun_ref_t(Result (T::*p)()): ptr(p) {}
    Result operator() (T &x) const {
        return x.*ptr();
    }
};

template <class Result, class T>
inline mem_fun_ref_t<Result, T> mem_fun_ref(Result (T::*p)()) {
    return mem_fun_ref_t<Result, T>(p);
}

template <class Result, class T>
struct const_mem_fun_ref_t: public unary_function<const T, Result> {
private:
    Result (T::*ptr)() const;
public:
    explicit const_mem_fun_ref_t(Result (T::*p)() const): ptr(p) {}
    Result operator() (T &x) const {
        return x.*ptr();
    }
};

template <class Result, class T>
inline mem_fun_ref_t<Result, T> mem_fun_ref(Result (T::*p)() const) {
    return const_mem_fun_ref_t<Result, T>(p);
}

template <class Result, class T, class A>
struct mem_fun1_t: public binary_function<T*, A, Result> {
private:
    Result (T::*ptr)(A);
public:
    explicit mem_fun1_t(Result (T::*p)(A)): ptr(p) {}
    Result operator() (T *x, A val) const {
        return x->*ptr(val);
    }
};

template <class Result, class T, class A>
inline mem_fun1_t<Result, T, A> mem_fun1(Result (T::*p)(A)) {
    return mem_fun1_t<Result, T, A>(p);
}

template <class Result, class T, class A>
struct const_mem_fun1_t: public binary_function<const T*, A, Result> {
private:
    Result (T::*ptr)(A) const;
public:
    explicit const_mem_fun1_t(Result (T::*p)(A) const ): ptr(p) {}
    Result operator() (const T *x, A val) const {
        return x->*ptr(val);
    }
};

template <class Result, class T, class A>
inline const_mem_fun1_t<Result, T, A> mem_fun(Result (T::*p)(A) const) {
    return const_mem_fun1_t<Result, T, A>(p);
}

template <class Result, class T, class A>
struct mem_fun1_ref_t: public binary_function<T, A, Result> {
private:
    Result (T::*ptr)(A);
public:
    explicit mem_fun1_ref_t(Result (T::*p)(A)): ptr(p) {}
    Result operator() (T &x, A val) const {
        return x.*ptr(val);
    }
};

template <class Result, class T, class A>
inline mem_fun1_ref_t<Result, T, A> mem_fun_ref(Result (T::*p)(A)) {
    return mem_fun1_ref_t<Result, T, A>(p);
}

template <class Result, class T, class A>
struct const_mem_fun1_ref_t: public binary_function<const T, A, Result> {
private:
    Result (T::*ptr)(A) const;
public:
    explicit const_mem_fun1_ref_t(Result (T::*p)(A) const): ptr(p) {}
    Result operator() (T &x, A val) const {
        return x.*ptr(val);
    }
};

template <class Result, class T, class A>
inline const_mem_fun1_ref_t<Result, T, A> mem_fun_ref(Result (T::*p)(A) const) {
    return const_mem_fun1_ref_t<Result, T, A>(p);
}

};

#endif
