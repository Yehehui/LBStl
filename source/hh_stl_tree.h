#ifndef __HH_STL_TREE_H_
#define __HH_STL_TREE_H_

#include "hh_stl_iterator.h"
#include "hh_stl_alloc.h"
#include "hh_config.h"
#include "hh_stl_construct.h"
#include "hh_stl_pair.h"
#include<cstddef>
#include<algorithm>

namespace hh_std {

typedef bool __rb_tree_node_color;
const __rb_tree_node_color red = true;
const __rb_tree_node_color black = false;

struct __rb_tree_node_base {
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node_color color_type;
    base_ptr parent;
    base_ptr left;
    base_ptr right;
    color_type  color;

    static base_ptr minimum(base_ptr root) {
        while(root->left) {
            root = root->left;
        }
        return root;
    }

    static base_ptr maxmum(base_ptr root) {
        while(root->right) {
            root = root->right;
        }
        return root;
    }
};

template <class Value>
struct __rb_tree_node: public __rb_tree_node_base {
    typedef __rb_tree_node<Value>* link_type;
    Value data;
};

struct __rb_tree_iterator_base {
    typedef __rb_tree_node_base::base_ptr base_ptr;
    typedef bidirectional_iterator_tag iterator_category;
    typedef ptrdiff_t difference_type;
    base_ptr node;
    __rb_tree_iterator_base() = default;
    void incre() {
        if (node->right) {
            node = node->right;
            while(node->left) {
                node = node->left;
            }
        } else {
            base_ptr y = node->parent;
            while(y->right == node) {
                node = y;
                y = node->parent;
            }
            if(node->right != y)
                node = y;
        }
    }
    void decre() {
        if(node->color == red && node->parent->parent == node) {
            node = node->right;
        } else if(node->left) {
            node = node->left;
            while(node->right) {
                node = node->right;
            }
        } else {
            base_ptr y = node->parent;
            while(y->left == node) {
                node = y;
                y = node->parent;
            }
            if(node->left != y)
                node = y;
        }
    }
};

template <class Value, class Ref, class Ptr>
struct __rb_tree_iterator: public __rb_tree_iterator_base {
    typedef Value value_type;
    typedef Ref reference;
    typedef Ptr pointer;
    typedef __rb_tree_iterator<Value, Value&, Value*> iterator;
    typedef __rb_tree_iterator<Value, const Value&, const Value*> const_iterator;
    typedef __rb_tree_iterator<Value, Ref, Ptr> self;
    typedef __rb_tree_node<Value>* link_type;

    __rb_tree_iterator() = default;
    __rb_tree_iterator(base_ptr x) {
        node = x;
    }
    __rb_tree_iterator(const __rb_tree_iterator &x) {
        node = x.node;
    }
    reference operator* () const {
        return ((link_type)node)->data;
    }
    pointer operator-> () const {
        return &(operator*());
    }
    self &operator++ () {
        incre();
        return *this;
    }
    self operator++ (int) {
        self tmp = *this;
        tmp.incre();
        return tmp;
    }
    self &operator-- () {
        decre();
        return *this;
    }
    self operator-- (int) {
        self tmp = *this;
        tmp.decre();
        return tmp;
    }

};

template <class Value, class Ref, class Ptr>
inline bool operator== (const __rb_tree_iterator<Value, Ref, Ptr> &x, const __rb_tree_iterator<Value, Ref, Ptr> &y) {
    return x.node == y.node;
}

template <class Value, class Ref, class Ptr>
inline bool operator!= (const __rb_tree_iterator<Value, Ref, Ptr> &x, const __rb_tree_iterator<Value, Ref, Ptr> &y) {
    return x.node != y.node;
}

inline void __rb_tree_rotate_left(__rb_tree_node_base *node, __rb_tree_node_base*& root) {
    __rb_tree_node_base *y = node->right;
    node->right = y->left;
    if(y->left) {
        y->left->parent = node;
    }
    if(node == root) {
        root = y;
    } else if(node == node->parent->left) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->parent = node->parent;
    y->left = node;
    node->parent = y;
}

inline void __rb_tree_rotate_right(__rb_tree_node_base *node, __rb_tree_node_base*& root) {
    __rb_tree_node_base *y = node->left;
    node->left = y->right;
    if(y->right) {
        y->right->parent = node;
    }
    if(node == root) {
        root = y;
    } else if(node->parent->left == node) {
        node->parent->left = y;
    } else {
        node->parent->right = y;
    }
    y->parent = node->parent;
    y->right = node;
    node->parent = y;
}

inline void __rb_tree_rebalance(__rb_tree_node_base *node, __rb_tree_node_base*& root) {
    node->color = red;
    while(node != root && node->parent->color == red) {
        if(node->parent == node->parent->parent->left) {
            __rb_tree_node_base *y = node->parent->parent->right;
            if(y && y->color == red) {
                node->parent->color = black;
                y->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            } else {
                if(node == node->parent->right) {
                    node = node->parent;
                    __rb_tree_rotate_left(node, root);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                __rb_tree_rotate_right(node->parent->parent, root);
            }
        } else {
            __rb_tree_node_base *y = node->parent->parent->left;
            if(y && y->color == red) {
                node->parent->color = black;
                y->color = black;
                node->parent->parent->color = red;
                node = node->parent->parent;
            } else {
                if(node == node->parent->left) {
                    node = node->parent;
                    __rb_tree_rotate_right(node, root);
                }
                node->parent->color = black;
                node->parent->parent->color = red;
                __rb_tree_rotate_left(node->parent->parent, root);
            }
        }
    }
    root->color = black;
}

inline __rb_tree_node_base *__rb_tree_rebalance_erase(__rb_tree_node_base *node, __rb_tree_node_base*& root, __rb_tree_node_base*& leftmost, __rb_tree_node_base*& rightmost) {
    __rb_tree_node_base *y = node, *x, *x_parent;
    if (node->left == 0) {
        x = node->right;
    } else {
        if(node->right == 0) {
            x = node->left;
        } else {
            y = node->right;
            while(y->left) {
                y = y->left;
            }
            x = y->right;
        }
    }
    if(y != node) {
        y->left = node->left;
        node->left->parent = y;
        if (y == node->right) {
            x_parent = y;
        } else {
            x_parent = y->parent;
            if(x) x->parent = y->parent;
            y->parent->left = x;
            y->right = node->right;
            node->right->parent = y;
        }
        if(node == root) {
            root = y;
        } else if(node == node->parent->left) {
            node->parent->left = y;
        } else {
            node->parent->right = y;
        }
        y->parent = node->parent;
        std::swap(y->color, node->color);
        y = node;
    } else {
        x_parent = node->parent;
        if(x) x->parent = node->parent;
        if(node == root) {
            root = x;
        } else if(node == node->parent->left) {
            node->parent->left = x;
        } else {
            node->parent->right = x;
        }
        if(node == leftmost) {
            if(x) {
                leftmost = __rb_tree_node_base::minimum(x);
            } else {
                leftmost = node->parent;
            }
        }
        if (node == rightmost) {
            if(x) {
                rightmost = __rb_tree_node_base::maxmum(x);
            } else {
                rightmost = node->parent;
            }
        }
    }
    if(y->color == black) {
        __rb_tree_node_base *w;
        while(x != root && (x == 0 || x->color == black)) {
            if(x == x_parent->left) {
                w = x_parent->right;
                if(w && w->color == red) {
                    x_parent->color = red;
                    w->color = black;
                    __rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                }
                if((w->left == 0 || w->left->color == black) && (w->right == 0 || w->right->color == black)) {
                    w->color = red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if(w->right == 0 || w->right->color == black) {
                        if(w->left) w->left->color = black;
                        w->color = red;
                        __rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = w->right->color = black;
                    __rb_tree_rotate_left(x_parent, root);
                    break;
                }
            } else {
                w = x_parent->left;
                if(w && w->color == red) {
                    x_parent->color = red;
                    w->color = black;
                    __rb_tree_rotate_right(x_parent, root);
                    w = x_parent->left;
                }
                if((w->left == 0 || w->left->color == black) && (w->right == 0 || w->right->color == black)) {
                    w->color = red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if(w->left == 0 || w->left->color == black) {
                        if(w->right) w->right->color = black;
                        w->color = red;
                        __rb_tree_rotate_left(w, root);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = w->right->color = black;
                    __rb_tree_rotate_right(x_parent, root);
                    break;
                }
            }
        }
        if(x) x->color = black;
    }
    return y;
}

template <class Value, class Key, class KeyofValue, class Compare, class Alloc = alloc>
class rb_tree {
protected:
    typedef __rb_tree_node_base* base_ptr;
    typedef __rb_tree_node<Value> rb_tree_node;
    typedef __rb_tree_node_color color_type;
    typedef simple_alloc<rb_tree_node, Alloc> data_allocate;
public:
    typedef Value value_type;
    typedef Key key_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef rb_tree_node* link_type;
    typedef size_t size_type;
    typedef ptrdiff_t difference_type;
protected:
    link_type get_node() {
        return data_allocate::allocate();
    }
    void put_node(link_type x) {
        data_allocate::deallocate(x);
    }
    link_type create_node(const value_type &x) {
        link_type result;
        __STL_TRY {
            result = get_node();
            construct(&(result->data), x);
        }
        __STL_UNWIND(destory(result));
        result->parent = result->left = result->right = 0;
        return result;
    }
    link_type clone_node(const link_type x) {
        link_type tmp = get_node();
        tmp->data = x->data;
        tmp->color = x->color;
        tmp->parent = tmp->left = tmp->right = 0;
        return tmp;
    }
    void destory_node(link_type x) {
        destory(&(x->data));
        put_node(x);
    }
protected:
    size_type  node_count;
    link_type  header;
    Compare comp;

    link_type &root() const {
        return (link_type&)header->parent;
    }
    link_type &leftmost() const {
        return (link_type&)header->left;
    }
    link_type &rightmost() const {
        return (link_type&)header->right;
    }

    static link_type &left(link_type x) {
        return (link_type&)x->left;
    }
    static link_type &right(link_type x) {
        return (link_type&)x->right;
    }
    static link_type &parent(link_type x) {
        return (link_type&)x->parent;
    }
    static value_type &value(link_type x) {
        return x->data;
    }
    static key_type &key(link_type x) {
        return (key_type&)KeyofValue()(value(x));
    }
    static color_type &color(link_type x) {
        return x->color;
    }

    static link_type &left(base_ptr x) {
        return (link_type&)(x->left);
    }
    static link_type &right(base_ptr x) {
        return (link_type&)(x->right);
    }
    static link_type &parent(base_ptr x) {
        return (link_type&)(x->parent);
    }
    static value_type &value(base_ptr x) {
        return value((link_type)x);
    }
    static key_type &key(base_ptr x) {
        return key((link_type)x);
    }
    static color_type &color(base_ptr x) {
        return color((link_type)x);
    }

    static link_type minimum(link_type node) {
        return (link_type)__rb_tree_node_base::minimum(node);
    }
    static link_type maxmum(link_type node) {
        return (link_type)__rb_tree_node_base::maxmum(node);
    }
public:
    typedef __rb_tree_iterator<value_type, reference, pointer> iterator;
    typedef __rb_tree_iterator<value_type, const_reference, const_pointer> const_iterator;

    typedef reverse_iterator<const_iterator> const_r_iterator;
    typedef reverse_iterator<iterator> r_iterator;

private:
    void init() {
        header = get_node();
        color(header) = red;
        parent(header) = 0;
        leftmost() = rightmost() = header;
    }
    iterator __insert(base_ptr x, base_ptr y, const value_type &v) {
        link_type x_ = link_type(x), y_ = link_type(y), z = create_node(v);
        if(y == header || x_ != 0 || comp(KeyofValue()(v), key(y))) {
            left(y) = z;
            if(y == leftmost()) {
                leftmost() = z;
            }
            if(y == header) {
                root() = z;
                rightmost() = z;
            }
        } else {
            right(y) = z;
            if(y == rightmost())
                rightmost() = z;
        }
        parent(z) = (link_type)y;
        left(z) = right(z) = 0;
        __rb_tree_rebalance(z, header->parent);
        ++node_count;
        return iterator(z);
    }
    link_type __copy(link_type x, link_type p) {
        link_type cur = clone_node(x);
        cur->parent = p;
        if(p == header) {
            root() = cur;
        }
        __STL_TRY {
            if(x->right) {
                cur->right = __copy((link_type)x->right, cur);
            }
            x = (link_type)x->left;
            p = cur;
            while(x) {
                p->left = clone_node(x);
                p->left->parent = p;
                p = (link_type)p->left;
                if(x->right) {
                    p->right = __copy((link_type)x->right, p);
                }
                x = (link_type)x->left;
            }
        }
        __STL_UNWIND(__erase(x));
        return cur;
    }
    void __erase(link_type x) {
        if(x == x->parent->left) {
            x->parent->left = 0;
        } else {
            x->parent->right = 0;
        }
        link_type y;
        while(x) {
            if (x->right) __erase((link_type)x->right);
            y = (link_type)x->left;
            destory_node(x);
            x = y;
        }
    }
    void __assign_recur(link_type x, link_type y, link_type p) {
        if(x == 0 && y == 0) return;
        if(x != 0 && y != 0) {
            x->data = y->data;
            x->color = y->color;
            __assign_recur((link_type)x->left, (link_type)y->left, x);
            __assign_recur((link_type)x->right, (link_type)y->right, x);
        } else if(x == 0 && y != 0) {
            x = __copy((link_type)y, p);
        } else {
            __erase(x);
        }
    }
public:
    rb_tree(const Compare c = Compare()): node_count(0), comp(c) {
        init();
    }
    rb_tree(const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &x): node_count(0), comp(x.comp) {
        header = get_node();
        header->color = red;
        __STL_TRY{
            if(x.root() == 0) {
                header->left = header->right = header;
                header->parent = 0;
            } else {
                root() = __copy(x.root(), header);
                leftmost() = (link_type)__rb_tree_node_base::minimum(root());
                rightmost() = (link_type)__rb_tree_node_base::maxmum(root());
            }
        }
        __STL_UNWIND(put_node(header));
        node_count = x.node_count;
    }
    ~rb_tree() {
        clear();
        put_node(header);
    }
    void clear() {
        if(node_count > 0) {
            __erase(root());
            node_count = 0;
            root() = 0;
            leftmost() = rightmost() = header;
        }
    }
    rb_tree<Value, Key, KeyofValue, Compare, Alloc> &operator= (const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &x) {
        if(this != &x) {
            if(x.root() == 0) {
                clear();
            } else {
                __assign_recur(root(), x.root(), header);
                leftmost() = (link_type)__rb_tree_node_base::minimum(root());
                rightmost() = (link_type)__rb_tree_node_base::maxmum(root());
                node_count = x.node_count;
            }
        }
        return *this;
    }
    Compare key_comp() const {
        return comp;
    }
    iterator begin() {
        return iterator(leftmost());
    }
    const_iterator begin() const {
        return const_iterator(leftmost());
    }
    iterator end() {
        return iterator(header);
    }
    const_iterator end() const {
        return const_iterator(header);
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
        return node_count == 0;
    }
    size_type size() const {
        return node_count;
    }
    size_type max_size() const {
        return size_type(-1);
    }
    void swap(rb_tree<Value, Key, KeyofValue, Compare, Alloc> &x) {
        std::swap(header, x.header);
        std::swap(node_count, x.node_count);
        std::swap(comp, x.comp);
    }
    hh_std::pair<iterator, bool> insert_unique(const value_type &x) {
        link_type cur = root(), p = header;
        while(cur) {
            p = cur;
            if(comp(KeyofValue()(x), key(cur))) {
                cur = (link_type)cur->left;
            } else {
                if(!comp(key(cur), KeyofValue()(x))) {
                    return hh_std::pair<iterator, bool>(iterator(cur), false);
                }
                cur = (link_type)cur->right;
            }
        }
        return hh_std::pair<iterator, bool>(__insert(cur, p, x), true);
    }
    iterator insert_equal(const value_type &x) {
        link_type cur = root(), p = header;
        while(cur) {
            p = cur;
            if(comp(KeyofValue()(x), key(cur))) {
                cur = (link_type)cur->left;
            } else {
                cur = (link_type)cur->right;
            }
        }
        return __insert(cur, p, x);
    }
    iterator insert_unique(iterator pos, const value_type &x) {
        if(pos.node == header->left) {
            if(size() > 0 && comp(KeyofValue()(x), key(pos.node))) {
                return __insert(pos.node, pos.node, x);
            }
            return insert_unique(x).first;
        } else if(pos.node == header) {
            if(comp(key(header->right), KeyofValue()(x))) {
                return __insert(0, pos.node, x);
            }
            return insert_unique(x).first;
        } else {
            iterator before = pos;
            --before;
            if(comp(key(before.node), KeyofValue()(x)) && comp(KeyofValue()(x), key(pos.node))) {
                if(right(before) == 0) {
                    return __insert(0, before.node, x);
                }
                return __insert(pos.node, pos.node, x);
            }
            return insert_unique(x).first;
        }
    }
    iterator insert_equal(iterator pos, const value_type &x) {
        if(pos.node == header->left) {
            if(size() > 0 && comp(KeyofValue()(x), key(pos.node))) {
                return __insert(pos.node, pos.node, x);
            }
            return insert_equal(x);
        } else if(pos.node == header) {
            if(!comp(KeyofValue()(x), key(header->right))) {
                return __insert(0, pos.node, x);
            }
            return insert_equal(x).first;
        } else {
            iterator before = pos;
            --before;
            if(!comp(KeyofValue()(x), key(before.node)) && !comp(key(pos.node), KeyofValue()(x))) {
                if(right(before) == 0) {
                    return __insert(0, before.node, x);
                }
                return __insert(pos.node, pos.node, x);
            }
            return insert_equal(x).first;
        }
    }
    template <class InputIterator>
    void insert_unique(InputIterator first, InputIterator last) {
        while(first != last) {
            insert_unique(*first);
            ++first;
        }
    }
    template <class InputIterator>
    void insert_equal(InputIterator first, InputIterator last) {
        while(first != last) {
            insert_equal(*first);
            ++first;
        }
    }
    void erase(iterator pos) {
        if(pos != end()) {
            __rb_tree_rebalance_erase(pos.node, header->parent, header->left, header->right);
            destory_node((link_type)pos.node);
            --node_count;
        }
    }
    size_type erase(const key_type & x) {
        hh_std::pair<iterator, iterator> range = equal_range(x);
        size_type result = distance(range.first, range.second);
        erase(range.first, range.second);
        return result;
    }
    void erase(iterator first, iterator last) {
        if(first == begin() && last == end()) {
            clear();
        } else {
            iterator tmp = first;
            while(first != last) {
                ++tmp;
                erase(first);
                first = tmp;
            }
        }
    }
    void erase(const key_type * first, const key_type * last) {
        while(first != last) {
            erase(*first);
            ++first;
        }
    }
    iterator find(const key_type & x) {
        link_type cur = root(), seleted = 0;
        while(cur) {
            if(!comp(key(cur), x)) {
                if(!comp(x, key(cur))) {
                    seleted = cur;
                }
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return seleted == 0 ? end() : iterator(seleted);
    }
    const_iterator find(const key_type & x) const {
        link_type cur = root(), seleted = 0;
        while(cur) {
            if(!comp(key(cur), x)) {
                if(!comp(x, key(cur))) {
                    seleted = cur;
                }
                cur = cur->left;
            } else {
                cur = cur->right;
            }
        }
        return seleted == 0 ? end() : const_iterator(seleted);
    }
    size_type count(const key_type & x) const {
        hh_std::pair<const_iterator, const_iterator> result = equal_range(x);
        difference_type len = distance(result.first, result.second);
        return len;
    }
    iterator lower_bound(const key_type & x) {
        link_type cur = root(), seleted = 0;
        while(cur) {
            if(!comp(key(cur), x)) {
                if (seleted == 0 || !comp(key(seleted), key(cur))) {
                    seleted = cur;
                }
                cur = (link_type)cur->left;
            } else {
                cur = (link_type)cur->right;
            }
        }
        return seleted == 0 ? end() : iterator(seleted);
    }
    const_iterator lower_bound(const key_type & x) const {
        link_type cur = root(), seleted = 0;
        while(cur) {
            if(!comp(key(cur), x)) {
                if (seleted == 0 || !comp(key(seleted), key(cur))) {
                    seleted = cur;
                }
                cur = (link_type)cur->left;
            } else {
                cur = (link_type)cur->right;
            }
        }
        return seleted == 0 ? end() : const_iterator(seleted);
    }
    iterator upper_bound(const key_type & x) {
        link_type cur = root(), seleted = 0;
        while(cur) {
            if(comp(x, key(cur))) {
                if(seleted == 0 || !comp(key(seleted), key(cur))) {
                    seleted = cur;
                }
                cur = (link_type)cur->left;
            } else {
                cur = (link_type)cur->right;
            }
        }
        return seleted == 0 ? end() : iterator(seleted);
    }
    const_iterator upper_bound(const key_type & x) const {
        link_type cur = root(), seleted = 0;
        while(cur) {
            if(comp(x, key(cur))) {
                if(seleted == 0 || !comp(key(seleted), key(cur))) {
                    seleted = cur;
                }
                cur = (link_type)cur->left;
            } else {
                cur = (link_type)cur->right;
            }
        }
        return seleted == 0 ? end() : const_iterator(seleted);
    }
    hh_std::pair<iterator, iterator> equal_range(const key_type & x) {
        return hh_std::pair<iterator, iterator>(lower_bound(x), upper_bound(x));
    }
    hh_std::pair<const_iterator, const_iterator> equal_range(const key_type & x) const {
        return hh_std::pair<const_iterator, const_iterator>(lower_bound(x), upper_bound(x));
    }
    bool rb_verify() const {
        if(node_count == 0) {
            return begin() == end() && header->left == header && header->right == header;
        }
        size_type len = __black_count(leftmost(), root());
        for(const_iterator it = begin(); it != end(); ++it) {
            link_type cur = (link_type)it.node;
            link_type l = (link_type)cur->left, r = (link_type)cur->right;
            if(cur->color == red) {
                if((l && l->color == red) || (r && r->color == red)) {
                    return false;
                }
            }
            if(l && comp(key(cur), key(l))) {
                return false;
            }
            if(r && comp(key(r), key(cur))) {
                return false;
            }
            if(!l && !r && __black_count(cur, root()) != len) {
                return false;
            }
        }
        if(leftmost() != __rb_tree_node_base::minimum(root())) {
            return false;
        }
        if(rightmost() != __rb_tree_node_base::maxmum(root())) {
            return false;
        }
        return true;
    }
};

template <class Value, class Key, class KeyofValue, class Compare, class Alloc>
inline bool operator== (const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &x, const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &y) {
    if(x.size() != y.size()) return false;
    typedef typename rb_tree<Value, Key, KeyofValue, Compare, Alloc>::iterator iterator;
    iterator first_x = x.begin(), first_y = y.begin();
    iterator last_x = x.end(), last_y = y.end();
    while(first_x != last_x && first_y != last_y) {
        if(*first_x != *first_y) {
            return false;
        }
        ++first_x;
        ++first_y;
    }
    return first_x == last_x && first_y == last_y;
}

template <class Value, class Key, class KeyofValue, class Compare, class Alloc>
inline bool operator< (const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &x, const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &y) {
    typedef typename rb_tree<Value, Key, KeyofValue, Compare, Alloc>::iterator iterator;
    iterator first_x = x.begin(), first_y = y.begin();
    iterator last_x = x.end(), last_y = y.end();
    while(first_x != last_x && first_y != last_y) {
        if(*first_x < *first_y) {
            return true;
        }
        if(*first_x > *first_y) {
            return false;
        }
        ++first_x;
        ++first_y;
    }
    return first_y != last_y;
}

template <class Value, class Key, class KeyofValue, class Compare, class Alloc>
inline void swap(const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &x, const rb_tree<Value, Key, KeyofValue, Compare, Alloc> &y) {
    x.swap(y);
}

inline size_t __black_count(__rb_tree_node_base *node, __rb_tree_node_base *root) {
    if(node == 0) return 0;
    size_t result = 0;
    while(true) {
        if(node->color == black) ++result;
        if(node == root) break;
        node = node->parent;
    }
    return result;
}

};

#endif
