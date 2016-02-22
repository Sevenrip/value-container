#pragma once

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include <string>
#include <unordered_map>
#include <iomanip>
#include "../Utils/MetaUtils.hpp"


template<class T>
struct DefaultValueHolder
{
protected:	
	DefaultValueHolder(const T & t) : _defaultValue(t) {};
	DefaultValueHolder(const DefaultValueHolder &)=delete;
	DefaultValueHolder & operator=(const DefaultValueHolder &) = delete;

	const T & _defaultValue;
};


template <class To>
struct GenericValueConverter : public DefaultValueHolder<std::shared_ptr<To>> {
	GenericValueConverter(const std::shared_ptr<To> & defaultvalue) : DefaultValueHolder<std::shared_ptr<To>>(defaultvalue) {}
	template <typename From, typename std::enable_if<!std::is_same<From,To>::value>::type* = nullptr>
	std::shared_ptr<To> operator()(const From & f){
		return this->_defaultValue;
	}
	
	template <typename From, typename std::enable_if<std::is_same<From,To>::value>::type* = nullptr>
	To operator()(const From & f){
		return f;
	}

	template <typename From, typename std::enable_if<std::is_same<From,To>::value>::type* = nullptr>
	std::shared_ptr<To> operator()(const std::shared_ptr<From> & f){
		return f;
	}

};

struct StringConverter : public DefaultValueHolder<std::shared_ptr<const std::string>>
{
	StringConverter(const std::shared_ptr<const std::string> & defaultString) :  DefaultValueHolder<std::shared_ptr<const std::string>>(defaultString) {}
	std::shared_ptr<const std::string> operator()(const std::shared_ptr<std::string> & s)
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
		return _defaultValue;
	}

	std::shared_ptr<const std::string> operator()(bool b)
	{
		std::stringstream stream;
		stream << (b ? "true" : "false");
		return std::make_shared<const std::string>(stream.str());
	}
	
};


template<typename To>
struct NumericConverter : public DefaultValueHolder<To>
{
	NumericConverter(const To & defaultNumeric) : DefaultValueHolder<To>(defaultNumeric) {}
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
		return this->_defaultValue;
	}

};


struct BoolConverter :public DefaultValueHolder<bool>
{
	BoolConverter(const bool & defaultBool) : DefaultValueHolder<bool>(defaultBool) {}
	template <typename From,typename std::enable_if<!std::is_arithmetic<From>::value>::type* = nullptr>
	bool operator()(const From & f)
	{
		return _defaultValue;
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
	using AdaptorType = GenericValueConverter<To>;
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





