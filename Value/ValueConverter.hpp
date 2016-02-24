#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include <string>
#include <unordered_map>
#include <iomanip>
#include  <limits>
#include "../Utils/MetaUtils.hpp"

template <class To>
struct GenericValueConverter {
    template <typename From, typename std::enable_if<!std::is_same<From,To>::value>::type* = nullptr>
    To operator()(const From & f){
        return To();
    }
    
    template <typename From, typename std::enable_if<std::is_same<From,To>::value>::type* = nullptr>
    To operator()(const From & f){
        return f;
    }
};

struct StringConverter
{
    std::shared_ptr<const std::string> operator()(const std::shared_ptr<const std::string> & s)
    {
        return s;
    }

    template <typename From, typename std::enable_if<std::is_arithmetic<From>::value>::type* = nullptr>
    std::shared_ptr<const std::string> operator()(const From & f)
    {
        std::stringstream stream;
        if(std::is_floating_point<From>::value) {
            stream << std::setprecision(std::numeric_limits<From>::max_digits10);
        }
        stream << f;
        return std::make_shared<const std::string>(stream.str());
    }

    template <typename From,  typename std::enable_if<!std::is_arithmetic<From>::value>::type* = nullptr>
    std::shared_ptr<const std::string> operator()(const From & t)
    {
        return std::make_shared<const std::string>();
    }

    std::shared_ptr<const std::string> operator()(bool b)
    {
        std::stringstream stream;
        stream << (b ? "true" : "false");
        return std::make_shared<const std::string>(stream.str());
    }
    
};


template<typename To>
struct NumericConverter
{
    template <typename From, typename std::enable_if<std::is_arithmetic<From>::value>::type* = nullptr>
    To operator()(const From & f)
    {
        return static_cast<To>(f);
    }
    
    To operator()(const std::shared_ptr<std::string> & s)
    {
        To nr;
        return (std::istringstream (*s) >> nr) ? nr : 0;
    }
    
    template <typename From, typename std::enable_if<!std::is_arithmetic<From>::value>::type* = nullptr>
    To operator()(const From & f)
    {
        return 0;
    }

};


struct BoolConverter
{
    template <typename From,typename std::enable_if<!std::is_arithmetic<From>::value>::type* = nullptr>
    bool operator()(const From & f)
    {
        return false;
    }
    
    template <typename From, typename std::enable_if<std::is_arithmetic<From>::value>::type* = nullptr>
    bool operator()(const From & f)
    {
        return f == 1;
    }
    
    bool operator()(const std::shared_ptr<const std::string> & s)
    {
        return (*s == "yes" || *s == "true");
    }
    

};


template <typename To, typename = void> struct ConverterAdaptor;

template<typename To>
struct ConverterAdaptor<To, typename std::enable_if<!std::is_arithmetic<To>::value>::type>
{
    using ConverterType = GenericValueConverter<To>;
};

template<>
struct ConverterAdaptor<bool>
{
    using ConverterType = BoolConverter;
};


template<>
struct ConverterAdaptor<std::string>
{
    using ConverterType = StringConverter;
};

template<typename To>
struct ConverterAdaptor<To, typename std::enable_if<std::is_arithmetic<To>::value && !is_bool<To>::value>::type>
{
    using ConverterType = NumericConverter<To>;
};





