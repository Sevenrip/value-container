#pragma once

#include <type_traits>
#include <memory>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <cstdlib>

template< class... >
using void_t = void;

template< class, class = void_t<> >
struct has_type_member : std::false_type { };

template <typename B>
struct has_type_member<B, void_t<typename B::type>> : std::true_type{};

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

template<typename>
struct is_bool : public std::false_type { };

template<>
struct is_bool<bool> : public std::true_type { };

template<>
struct is_bool<const bool>: public std::true_type { };

template<>
struct is_bool<volatile bool> : public std::true_type { };



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

