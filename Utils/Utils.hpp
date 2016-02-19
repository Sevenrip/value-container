#pragma once

#include <chrono>
#include <memory>
#include <random>
#include <algorithm> 
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <string>
#include <cstdlib>
#include <vector>

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <unordered_map>
#include <string>
#include <memory>

template<typename T>
using StringMap =  std::unordered_map<std::shared_ptr<const std::string> , std::shared_ptr<const T> >;

template<typename T>
using StringMapEntry = std::pair<std::shared_ptr<const std::string>, std::shared_ptr<const T> >;

template<class T, class U=
  typename std::remove_cv<
  typename std::remove_pointer<
  typename std::remove_reference<
  typename std::remove_extent<
  T
  >::type
  >::type
  >::type
  >::type
  > struct remove_all : remove_all<U> {};
template<class T> struct remove_all<T, T> { typedef T type; };



template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}


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