#pragma once

#include <memory>
#include <iostream>
#include <type_traits>
#include <vector>
#include "variant/recursive_wrapper.hpp"
#include "variant/variant.hpp"
#include "ValueConverter.hpp"


using namespace mapbox::util;


class Value
{
	
	
public:
	using Vector = std::vector<std::shared_ptr<const Value>>;
	using StringMap = std::unordered_map<std::shared_ptr<const std::string>, std::shared_ptr<const Value>>;
	using StringMapEntry = std::pair<std::shared_ptr<const std::string>, std::shared_ptr<const Value>>;
    using values = variant<std::shared_ptr<const std::string> ,  unsigned int, float, bool, std::shared_ptr<const Vector>, std::shared_ptr<const StringMap>  >;

	
	Value() {}
	template <typename Arg, typename std::enable_if<std::is_arithmetic<Arg>::value &&
													std::is_constructible<values, Arg>::value >::type * = nullptr>
	explicit Value(const Arg & arg) : _holder(static_cast<Arg>(arg)) //dodging those sneaky compiler arithmetic type conversions
	{
	}
	
	template <typename Arg, typename std::enable_if<std::is_constructible<values, std::shared_ptr<typename std::remove_reference<Arg>::type> >::value >::type * = nullptr>
	explicit Value(Arg && arg) : _holder(std::make_shared<typename std::remove_reference<Arg>::type>(std::forward<Arg>(arg)))
	{
	}
	
	explicit Value(const char * s) : _holder(std::make_shared<std::string>(s)) {}
	
	template<typename Arg>
	Value & operator=(Arg && arg)
	{
		static_assert(std::is_constructible<Value, Arg>::value, "Type requested to convert is not valid");		
		
		Value tmp(std::forward<Arg>(arg));
		using std::swap; //enables ADL in case we use a future variant version that includes a custom swap
		swap(*this,tmp);
		
		return *this;
	}
    	
	bool operator!= (const Value& value) const noexcept;
    bool operator== (const Value& value) const noexcept;
    
    
    friend std::ostream & operator<<(std::ostream & stream, const Value & v);

    std::string description(int depth = 0) const;
    
    std::shared_ptr<const std::string> asString() const noexcept;
	unsigned int asUnsignedInt() const noexcept;
    int asInt() const noexcept;
	bool asBool() const noexcept;
	float asFloat() const noexcept;
    std::shared_ptr<const Vector> asVector() const noexcept;
    std::shared_ptr<const StringMap> asMap() const noexcept;
	
	bool isNull() const noexcept;
	bool isUnsignedInt() const noexcept;
	bool isBool() const noexcept;
	bool isFloat() const noexcept;
	bool isString() const noexcept;
	bool isVector() const noexcept;
	bool isMap() const noexcept;
	
private:
		values _holder;
		
		template<typename To> using ResolveReturnConvertType =  decltype(mapbox::util::apply_visitor(typename ConverterAdaptor<To>::ConverterType(), _holder));
	
public:
	template<typename To>
	auto convertTo() const noexcept ->  ResolveReturnConvertType<To>
	{
		static_assert(std::is_constructible<Value, To>::value, "Type requested to convert is not valid");
		using ReturnType =  ResolveReturnConvertType<To>;
		return  _holder.no_static_assert_is<ReturnType>() ? _holder.get<ReturnType>() : mapbox::util::apply_visitor(typename ConverterAdaptor<To>::ConverterType(), _holder);
	}
    



};






