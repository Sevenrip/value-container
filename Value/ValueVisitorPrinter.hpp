#pragma once

#include <ostream>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <string>
#include <sstream>
#include "../Utils/Utils.hpp"
#include "../Utils/MetaUtils.hpp"
#include "Value.hpp"


struct ValueVisitorPrinter
{
	
    std::string operator()(bool b ) 
    {
		return b ? "true" : "false";
    }

	template <typename T, typename std::enable_if<!supports_output_operator<T>::value>::type * = nullptr>
    std::string operator()(const T & t ) 
    {
		return "";
    }
	
	template <typename T, typename std::enable_if<supports_output_operator<T>::value && !supports_dereference<T>::value>::type * = nullptr>
    std::string operator()(const T & t )
    {
		std::stringstream stream;
		stream << t;
		return stream.str();
    }
	
	template <typename T>
    std::string operator()(const std::shared_ptr<T> & t )
    {
        return this->operator()(*t);
    }
	
	
    std::string operator()(const std::string & s ) 
    {
        return s;
    }

	template <typename T>
    std::string operator()(const std::vector<T> & t)
    {
		std::string str = "[ ";
		for(const auto & el : t) {
			str += this->operator()(el);
		}
		str+=" ]";
		return str;
    }
	
	template <typename T, typename U>
	std::string operator()(const std::unordered_map<T, U> & map)
	{
		std::string str = "{ " ;
		for(const auto & pair : map) {
			str+= "{ " ;
			str += this->operator()(pair.first);
			str += " : " ;
			str += this->operator()(pair.second);
			str+= "} " ;
		}
		str+= " }";
		return str;
	}
	
private:
	int _depth;
};
