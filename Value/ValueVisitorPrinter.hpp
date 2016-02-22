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

class ValueVisitorPrinter
{
public:
	ValueVisitorPrinter(int depth) : _depth(depth) {};

	ValueVisitorPrinter (const ValueVisitorPrinter&) = delete;
	ValueVisitorPrinter & operator=(const ValueVisitorPrinter&) = delete;

	template<typename T>
	std::string operator()(const T & t) {
		return visit(t, _depth);
	}

private:
	std::string visit(const Value & v, int depth = 0)
	{
		return v.description(depth);
	}

	template <typename T, typename std::enable_if<!supports_output_operator<T>::value>::type * = nullptr>
    std::string visit(const T & t, int depth = 0) 
    {
		return "";
    }
	
	template <typename T, typename std::enable_if<supports_output_operator<T>::value && !supports_dereference<T>::value>::type * = nullptr>
    std::string visit(const T & t, int depth = 0 )
    {
		std::stringstream stream;
		if(is_bool<T>::value) {
			stream << std::boolalpha;
		}
		stream << t;
		return stream.str();
    }
	
	template <typename T>
    std::string visit(const std::shared_ptr<T> & t, int depth = 0 )
    {
        return visit(*t, depth);
    }
	
	
    std::string visit(const std::string & s, int depth = 0 ) 
    {
        return s;
    }

	template <typename T>
    std::string visit(const std::vector<T> & t, int depth = 0)
    {
    	std::stringstream stream;

		stream << getTabs(depth) << "[";
		for(int i = 0; i < t.size(); i++) {
			stream << " " << i << ": " << visit(t.at(i), depth + 1);
		}
		stream  << " ]";
		return stream.str();
    }
	
	template <typename K, typename V>
	std::string visit(const std::unordered_map<K, V> & map, int depth = 0)
	{
		std::stringstream stream;
		stream << getTabs(depth) << "{" << std::endl;
		for(const auto & pair : map) {
			stream << getTabs(depth + 1) << "{" << visit(pair.first, depth) << ": ";
			stream << visit(pair.second, depth + 1);
		}
		stream << getTabs(depth) << "}" << std::endl;
		return stream.str();
	}

	static std::string getTabs(int depth)
    {
    	return depth == 0 ? "" : std::string("\t", depth);
    }

    int _depth;
};
