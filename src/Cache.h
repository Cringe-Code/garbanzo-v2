#pragma once

#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>


template <typename T>
class MyCache {
public:
    MyCache (int64_t MaxSize) : Sz(0), MaxSize(MaxSize) {};

    void insert(T &x, const std::string &id);
    
    T get(const std::string &id);
    
    bool exists(const std::string &id);
private:
    std::unordered_map<std::string, T> mp;
    int64_t Sz;
    int64_t MaxSize;
};

template <typename T>
void MyCache<T>::insert(T &x, const std::string &id) {
    mp[id] = x;
    ++Sz;

    if (Sz > MaxSize) {
        mp.erase(mp.begin());
    }
}

template <typename T>
bool MyCache<T>::exists(const std::string &id) {
    return mp.find(id) != mp.end();
}

template <typename T>
T MyCache<T>::get(const std::string &id) {
    if (exists(id)) {
        return mp[id];
    }

    return T();
}