#pragma once

#include <chrono>
#include <memory>
#include <random>
#include <algorithm> 
#include <string>
#include <cstdlib>
#include <vector>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unordered_map>
#include <string>

template<typename T>
using StringMap =  std::unordered_map<std::shared_ptr<const std::string> , std::shared_ptr<const T> >;

template<typename T>
using StringMapEntry = std::pair<std::shared_ptr<const std::string>, std::shared_ptr<const T> >;

template<typename TimeT = std::chrono::milliseconds>
struct measure
{
    template<typename F, typename ...Args>
    static typename TimeT::rep execution(F&& func, Args&&... args)
    {
        auto start = std::chrono::system_clock::now();
        std::forward<decltype(func)>(func)(std::forward<Args>(args)...);
        auto duration = std::chrono::duration_cast< TimeT> 
                            (std::chrono::system_clock::now() - start);
        return duration.count();
    }
};

std::string random_string( size_t length )
{
    auto randchar = []() -> char
    {
        const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}