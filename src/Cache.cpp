#include <cstdint>

#include "Cache.h"

template <typename T>
void MyCache<T>::insert(T &x, int64_t &id) {
    mp[id] = x;
    ++Sz;

    if (Sz > MaxSize) {
        mp.erase(mp.begin());
    }
}

template <typename T>
bool MyCache<T>::exists(int64_t &id) {
    return mp.find(id) != mp.end();
}

template <typename T>
T MyCache<T>::get(int64_t &id) {
    if (exists(id)) {
        return mp[id];
    }

    return T();
}
