#pragma once

#include <sstream>
#include <memory>
#include <type_traits>
#include <string>
#include "../Utils/Utils.hpp"
#include "Value.hpp"

template <class To>
struct BasicValueConverter {
	template <typename From, typename std::enable_if<!std::is_same<From,To>::value>::type* = nullptr>
	To operator()(const From & f){
		return To();
	}
	
	template <typename From, typename std::enable_if<std::is_same<From,To>::value>::type* = nullptr>
	To operator()(const From & f){
		return f;
	}

	template <typename From, typename std::enable_if<std::is_same<typename std::shared_ptr<To>,From>::value>::type* = nullptr>
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

	template <typename T, typename std::enable_if< std::numeric_limits<T>::is_integer>::type* = nullptr>
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
	template <typename T,  typename std::enable_if<is_bool<T>::value>::type* = nullptr>
	std::shared_ptr<const std::string> operator()(const T  & b)
	{
		std::stringstream stream;
		stream << (static_cast<bool>(b) ? "true" : "false");
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
	
	bool operator()(const std::shared_ptr<std::string> & s)
	{
		
		return (*s == "yes" || *s == "true");
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
	using AdaptorType = StringConverter;
};

template<typename To>
struct ConverterAdaptor<To, typename std::enable_if<std::is_arithmetic<To>::value>::type>
{
	using AdaptorType = NumericConverter<To>;
};





