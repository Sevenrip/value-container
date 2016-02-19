#pragma once

#include <memory>
#include <iostream>
#include <functional>
#include <type_traits>
#include <vector>
#include "../variant/recursive_wrapper.hpp"
#include "../variant/variant.hpp"
#include "../variant/variant_io.hpp"
#include "../Utils/Utils.hpp"
#include "ValueVisitorEqual.hpp"
#include "ValueStringConverter.hpp"
#include "ValueConverter.hpp"

using namespace mapbox::util;


class Value
{
	
	
public:
	using Vector = std::vector<std::shared_ptr<const Value>>;
	using StringMap = std::unordered_map<std::shared_ptr<const std::string>, std::shared_ptr<const Value>>;
	using StringMapEntry = std::pair<std::shared_ptr<const std::string>, std::shared_ptr<const Value>>;
	using values = variant<std::shared_ptr<std::string>,unsigned int,  float, std::shared_ptr<std::vector<int>>, std::shared_ptr<StringMap>  >;

	
	Value() {}
	template <typename Arg, typename std::enable_if<std::is_fundamental<Arg>::value &&
													std::is_constructible<values, Arg>::value >::type * = nullptr>
	explicit Value(const Arg & arg) : _v(arg)
	{
	}
	
	template <typename Arg, typename std::enable_if<std::is_constructible<values, std::shared_ptr<typename std::remove_reference<Arg>::type> >::value >::type * = nullptr>
	explicit Value(Arg && arg) : _v(std::make_shared<typename std::remove_reference<Arg>::type>(std::forward<Arg>(arg)))
	{
	}
	
	Value(const char * s) : _v(std::make_shared<std::string>(s)) {}
	Value(bool b) : _v(b) {} //goddam int conversions
    
    
    Value &
		
	bool operator!= (const Value& value) noexcept
    {
        return !(*this == value);
    }
    bool operator!= (const Value& value) const noexcept
    {
        return !(*this == value);
    }
    
    bool operator== (const Value& value) noexcept
    {
        const auto &t = *this;
        return t == value;
    }
    
    bool operator== (const Value& value) const noexcept
	{
		return mapbox::util::apply_visitor(ValueVisitorEqual(),value._v, _v) ;
	}
    
    std::ostream & operator<<(std::ostream & stream, const Value & v)
    {
        
    }
	
	public:
		values _v;
	
	public:
		template<typename To>
		auto convertTo() -> decltype(mapbox::util::apply_visitor(typename ConverterAdaptor<To>::AdaptorType(), _v))
		{
			using ToValueConverter = typename ConverterAdaptor<To>::AdaptorType;
			return mapbox::util::apply_visitor(ToValueConverter(), _v);
		}


	
	/*template <typename Arg, typename std::enable_if<is_constructible<values, Arg>::value ||
											   is_constructible<values, shared_ptr<Arg> >::value >::type * = nullptr>
	Value & operator=(Arg && arg)
	{
		_v = forward<Arg>(arg);
	}
	
	Value & operator=(const char * s)
	{
		_v = make_shared<std::string>(s);
	}*/



};






