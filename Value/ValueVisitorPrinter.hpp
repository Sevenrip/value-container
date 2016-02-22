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
	

	template <typename T, typename std::enable_if<!supports_output_operator<T>::value>::type * = nullptr>
    std::string operator()(const T & t, int depth = 0) 
    {
		return "";
    }
	
	template <typename T, typename std::enable_if<supports_output_operator<T>::value && !supports_dereference<T>::value>::type * = nullptr>
    std::string operator()(const T & t, int depth = 0 )
    {
		std::stringstream stream;
		if(is_bool<T>::value) {
			stream << std::boolalpha;
		}
		stream << t;
		return stream.str();
    }
	
	template <typename T>
    std::string operator()(const std::shared_ptr<T> & t, int depth = 0 )
    {
        return this->operator()(*t, depth);
    }
	
	
    std::string operator()(const std::string & s, int depth = 0 ) 
    {
        return s;
    }

	template <typename T>
    std::string operator()(const std::vector<T> & t, int depth = 0)
    {
    	std::stringstream stream;
    	if(depth > 0) {
    		stream << std::endl;
    	}
		stream << getTabs(depth) << "[ " << std::endl;
		for(const auto & val : t) {
			stream << getTabs(depth + 1) << this->operator()(val, depth + 1);
		}
		stream << std::endl << "]";
		return stream.str();
    }
	
	template <typename K, typename V>
	std::string operator()(const std::unordered_map<K, V> & map, int depth = 0)
	{
		std::stringstream stream;
		stream << getTabs(depth) << "{" << std::endl;
		for(const auto & pair : map) {
			stream << getTabs(depth + 1) << "{" << this->operator()(pair.first, depth) << ": ";
			stream << this->operator()(pair.second, depth + 1);
		}
		stream << getTabs(depth) << "}" << std::endl;
		return stream.str();
	}

	
private:
	static std::string getTabs(int depth)
    {
    	return depth == 0 ? "" : std::string("\t", depth);
    }
};
