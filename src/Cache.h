#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <unordered_map>


template <typename T>
class MyCache {
public:
    MyCache (int64_t MaxSize) : Sz(0), MaxSize(MaxSize) {};

    void insert(T &x, int64_t &id);
    
    T get(int64_t &id);
    
    bool exists(int64_t &id);
private:
    std::unordered_map<int64_t, T> mp;
    int64_t Sz;
    int64_t MaxSize;
};
