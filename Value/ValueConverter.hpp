#pragma once

#include <sstream>
#include <memory>
#include <type_traits>
#include <string>
#include "../Utils/Utils.hpp"
#include "Value.hpp"

template <class To>
struct BasicValueConverter {
	template <typename From>
	To operator()(const From & f){
		//static_assert(false, "Not a convertible Type..");
		return To();
	}
	
	template <typename From, typename std::enable_if<std::is_same<From,To>::value>::type* = nullptr>
	To operator()(const From & f){
		return f;
	}

	template <typename From, typename std::enable_if<std::is_same<From,To>::value>::type* = nullptr>
	std::shared_ptr<To>  operator()(const From & f){
		return f;
	}		
};

struct StringConverter
{
	std::shared_ptr<const std::string> operator()(const std::shared_ptr<std::string> & s)
	{
		return s;
	}

	template <typename T, typename std::enable_if<std::is_integer<T>::value>::type* = nullptr>
	std::shared_ptr<const std::string> operator()(const T & t)
	{
		std::stringstream stream;
        stream << t;
		return std::make_shared<const std::string>(stream.str());
	}
	
	template <typename T, class = typename std::enable_if<std::is_floating_point<T>::value>::type>
	std::shared_ptr<const std::string> operator()(const T & t)
	{
		std::stringstream stream;
		stream << std::setprecision(std::numeric_limits<double>::max_digits10) << t;
		return std::make_shared<const std::string>(stream.str());
	}
	
	template <typename T,  typename std::enable_if<!std::is_arithmetic<T>::value>::type* = nullptr>
	std::shared_ptr<const std::string> operator()(const T & t)
	{
		return std::make_shared<const std::string>();
	}
	
	std::shared_ptr<const std::string> operator()(bool  b)
	{
		std::stringstream stream;
		stream << (b ? "true" : "false");
		return std::make_shared<const std::string>(stream.str());
	}
};

template<typename To>
struct IntegralConverter
{
	template <typename From, typename std::enable_if<std::is_arithmetic<From>::value>::type* = nullptr>
	To operator()(const From & f)
	{
		return static_cast<To>(f);
	}
	
	template <typename From, typename std::enable_if<!std::is_arithmetic<From>::value>::type* = nullptr>
	To operator()(const From & f)
	{
		return 0;
	}

};
template<typename To>
struct FloatingPointConverter
{

	template <typename From, typename std::enable_if<std::is_arithmetic<From>::value>::type* = nullptr>
	To operator()(const From & f)
	{
		std::cout << "floating converter" << std::endl;
		return static_cast<To>(f);
	}
	
	template <typename From, typename std::enable_if<!std::is_arithmetic<From>::value>::type* = nullptr>
	To operator()(const From & f)
	{
		std::cout << "random float shit converter" << std::endl;
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
	
	bool operator()(const std::shared_ptr<std::string> & s)
	{
		return (*s == std::string("yes"));
	}
	

};


template <typename To, typename = void> struct ConverterAdaptor;

template<typename To>
struct ConverterAdaptor<To, typename std::enable_if<!std::is_arithmetic<To>::value>::type>
{
	using AdaptorType = BasicValueConverter<To>;
};

template<>
struct ConverterAdaptor<bool>
{
	using AdaptorType = BoolConverter;
};


template<>
struct ConverterAdaptor<std::string>
{
	using AdaptorType = ValueStringConverter;
};

template<typename To>
struct ConverterAdaptor<To, typename std::enable_if<std::is_arithmetic<To>::value>::type>
{
	using AdaptorType = typename std::conditional<std::numeric_limits<To>::is_integer, 
												  IntegralConverter<To>, 
												  FloatingPointConverter<To> 
												  >::type;
};





