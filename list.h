//
// Created by ASUS on 22.06.2019.
//

#ifndef List_List_H
#define List_List_H

#include <iostream>

template <class T>
struct List {
    struct node {
        node* left;
        node* right;
        node(node* left, node* right) : left(left), right(right) {}
        node() : left(this), right(this) {}
        const bool empty() const {
            return (this == left) && (this == right);
        }
        ~node() = default;
    };
    struct extended_node : node {
        T value;
        extended_node() = default;
        ~extended_node() = default;
        extended_node(T const& v, node* l, node* r) : node(l, r), value(v) {}
    };

    template <class U>
    struct Iterator : std::iterator<std::bidirectional_iterator_tag, U, std::ptrdiff_t, U*, U&> {
        using difference_type = std::ptrdiff_t;
        typedef U value_type;
        typedef U* pointer;
        typedef U& reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        template<typename> friend struct const_iterator;
        friend class List;

        Iterator() = default;
        ~Iterator() = default;
        Iterator& operator++() {
            ptr = ptr->right;
            return *this;
        }
        const Iterator operator++(int) {
            Iterator res(ptr);
            ++(*this);
            return res;
        }
        Iterator& operator--() {
            ptr = ptr->left;
            return *this;
        }
        const Iterator operator--(int) {
            Iterator res(ptr);
            --(*this);
            return res;
        }
        U& operator *() const {
            return (static_cast<extended_node*>(ptr)->value);
        }
        U* operator ->() const {
            return &(static_cast<extended_node*>(ptr)->value);
        }

        friend bool operator ==(Iterator const& a, Iterator const& b) {
            return a.ptr == b.ptr;
        }

        friend bool operator !=(Iterator const& a, Iterator const& b) {
            return a.ptr != b.ptr;
        }

    private:
        explicit Iterator(List<T>::node* p) : ptr(p) {}
        node* ptr;
    };

    template <class U>
    struct const_Iterator : std::iterator<std::bidirectional_iterator_tag, const U, std::ptrdiff_t, const U*, const U&> {
        using difference_type = std::ptrdiff_t;
        typedef const U value_type;
        typedef const U* pointer;
        typedef const U& reference;
        typedef std::bidirectional_iterator_tag iterator_category;
        friend class List;

        const_Iterator() = default;
        const_Iterator(Iterator<T> const& p) : ptr(p.ptr) {}
        ~const_Iterator() = default;
        const_Iterator& operator++() {
            ptr = ptr->right;
            return *this;
        }
        const const_Iterator operator++(int) {
            const_Iterator res(ptr);
            ++(*this);
            return res;
        }
        const_Iterator& operator--() {
            ptr = ptr->left;
            return *this;
        }
        const const_Iterator operator--(int) {
            const_Iterator res(ptr);
            --(*this);
            return res;
        }
        const U& operator *() const {
            return (static_cast<extended_node*>(ptr)->value);
        }
        const U* operator ->() const {
            return &(static_cast<extended_node*>(ptr)->value);
        }

        friend bool operator ==(const_Iterator const& a, const_Iterator const& b) {
            return a.ptr == b.ptr;
        }

        friend bool operator !=(const_Iterator const& a, const_Iterator const& b) {
            return a.ptr != b.ptr;
        }

    private:
        explicit const_Iterator(List<T>::node* p) : ptr(p) {}
        node* ptr;
    };

    List() noexcept {}
    ~List() noexcept {
        clear();
    }

    List(List const& that) : List() {
        for (auto x = that.cbegin(); x != that.cend(); ++x) {
            push_back(*x);
        }
    }

    List& operator=(List const& that) {
        if (&that.fake != &fake) {
            clear();
            for (auto x = that.cbegin(); x != that.cend(); ++x) {
                push_back(*x);
            }
            return *this;
        }
        return *this;
    }

    typedef Iterator<T> iterator;
    using const_iterator = const_Iterator <T>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        return ++end();
    }

    iterator end() {
        return iterator(&fake);
    }

    const_iterator begin() const {
        return ++end();
    }

    const_iterator end() const {
        return const_iterator(const_cast<node*>(&fake));
    }
    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }
    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(cend());
    }
    reverse_iterator rend() {
        return reverse_iterator(begin());
    }
    const_reverse_iterator rend() const {
        return const_reverse_iterator(cbegin());
    }

    const_iterator cend() const noexcept{
        return end();
    }

    const_iterator cbegin() const noexcept {
        return begin();
    }

    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(cbegin());
    }

    void push_back(const T& value) {
        insert(end(), value);
    }


    void push_front(const T& value) {
        insert(begin(), value);
    }

    void pop_back() {
        erase(--end());
    }


    void pop_front() {
        erase(begin());
    }

    iterator erase(const_iterator that) {
        node * cur = that.ptr;
        node * next = cur->right;
        cur->left->right = next;
        next->left = cur->left;
        dealloc(static_cast<extended_node *>(cur));
        return iterator(next);
    }

    void splice(const_iterator pos, List& other, const_iterator first, const_iterator last) {
        if (first == last) {
            return;
        }
        node* a = pos.ptr;
        node* b = first.ptr;
        node* c = last.ptr->left;
        b->left->right = c->right;
        c->right->left = b->left;
        a->left->right = b;
        b->left = a->left;
        c->right = a;
        a->left = c;
    }

    void dealloc(extended_node* that) {
        delete that;
    }

    iterator insert(const_iterator that, T const& val) {
        node *cur = that.ptr;
        node *pred = cur->left;
        node *new_n = nullptr;
        try {
            //std::cout << val << " ";
            new_n = new extended_node(val, pred, cur);
        } catch (...) {
            //std::cout << "pisos" << std::endl;
            throw;
        }
        pred->right = new_n;
        cur->left = new_n;
        return iterator(new_n);
    }

    T& front() {
        return (*begin());
    }

    const T& front() const {
        return (*begin());
    }

    T& back() {
        return *--end();
    }

    const T& back() const {
        return *--cend();
    }


    friend void swap(List<T> & a, List<T> & b) noexcept {
        bool x1 = a.empty();
        bool x2 = b.empty();
        std::swap(a.fake.right, b.fake.right);
        std::swap(a.fake.left, b.fake.left);
        if (!x2) {
            a.fake.right->left = &a.fake;
            a.fake.left->right = &a.fake;
        } else {
            a.fake.right = &a.fake;
            a.fake.left = &a.fake;
        }
        if (!x1) {
            b.fake.right->left = &b.fake;
            b.fake.left->right = &b.fake;
        } else {
            b.fake.right = &b.fake;
            b.fake.left = &b.fake;
        }
    }

    void clear() {
        while (!empty()) {
            pop_back();
        }
    }

    const bool empty() const {
        return fake.empty();
    }

private:
    node fake;
};

#endif //List_List_H
