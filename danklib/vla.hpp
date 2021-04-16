#pragma once

#include <stdlib.h>
#include <functional>

template<typename T>
struct vla {
    const static int default_backing_size = 10;

    T *items;
    int backing_size;
    int length;

    int iter_pos = 0;

    vla() {
        items = (T *)malloc(sizeof(T) * default_backing_size);
        backing_size = default_backing_size;
        length = 0;
    }

    vla(T *i_items, int i_backing_size, int i_length) {
        items = i_items;
        length = i_length;
        backing_size = i_backing_size;
    }

    // to move just assign
    vla<T> deep_copy() const {
        T * new_items = (T *)malloc(sizeof(T) * backing_size);
        memcpy(new_items, items, sizeof(T) * length);

        return vla(new_items, backing_size, length);
    }

    void destroy() {
        if (items) {
            free(items);
            items = NULL;
        }
    }

    void push(T item) {
        if (length == backing_size - 1) { 
            backing_size *= 2;
            items = (T *)realloc(items, backing_size * sizeof(T));
        }

        items[length++] = item;
    }

    T pop_back() {
        if (length == 0) {
            printf("tried to pop empty vla\n");
            exit(1);
        }
        length--;
        return items[length];
    }

    void for_each(std::function<void(T)> f) {
        for (int i = 0; i < length; i++) {
            f(items[i]);
        }
    }

    void for_each_mut(std::function<void(T*)> f) {
        for (int i = 0; i < length; i++) {
            f(&items[i]);
        }
    }

    bool any(std::function<bool(T)> f) {
        for (int i = 0; i < length; i++) {
            if (f(items[i])) return true;
        }
        return false;
    }

    bool all(std::function<bool(T)> f) {
        for (int i = 0; i < length; i++) {
            if (!f(items[i])) return false;
        }
        return true;
    }

    int max(std::function<int (T)> f) {
        int max_idx = 0;
        int curr_max = f(items[0]);
        for (int i = 1; i < length; i++) {
            const auto curr_val = f(items[i]);
            if (curr_val > curr_max) {
                max_idx = i;
                curr_max = curr_val;
            }
        }
        return curr_max;
    }

    int acc(std::function<int(T)> f) {
        int count = 0;
        for (int i = 0; i < length; i++) {
            count += f(items[i]);
        }
        return count;
    }

    T *begin() {
        iter_pos = 0;
        return &items[iter_pos];
    }

    bool is_next() {
        return iter_pos < length - 1;
    }

    T *next() {
        iter_pos++;
        return &items[iter_pos];
    }
};