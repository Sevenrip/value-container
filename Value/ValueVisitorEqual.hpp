#pragma once

#include <ostream>
#include <memory>
#include <type_traits>
#include <string>
#include "../Utils/Utils.hpp"

template<typename T>
using StringMap =  std::unordered_map<std::shared_ptr<const std::string> , std::shared_ptr<const T> >;

template<typename T>
using StringMapEntry = std::pair<std::shared_ptr<const std::string>, std::shared_ptr<const T> >;

struct ValueVisitorEqual
{
    template <typename T, typename U, class = typename std::enable_if<!std::is_same<T,U>::value>::type>
    bool operator()(const T & lhs, const U & rhs) const
    {
        return false;
    }
	
	template <typename T,  class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    bool operator()(const T & lhs, const T & rhs) const
    {
        return lhs == rhs;
    }
	
	template <typename T>
    bool operator()(const std::shared_ptr<T> & lhs, const std::shared_ptr<T> & rhs) const
    {
        return *lhs == *rhs;
    }
	
	template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    bool operator()(const std::shared_ptr<std::vector<T>> & lhs, const std::shared_ptr<std::vector<T>> rhs) const
    {
		if(lhs->size() != rhs->size())
			return false;
		
        return std::equal(lhs->cbegin(), lhs->cend(), rhs->cbegin(), 
				[](const T & lhs, const T & rhs) {
					std::cout << std::boolalpha << (lhs == rhs) << std::endl;
					return lhs == rhs;
				});
    }
	template <typename T>
	bool operator()(const std::shared_ptr<StringMap<T>> & lhs, const std::shared_ptr<StringMap<T>> & rhs)
	{
		if(lhs->size() != rhs->size())
			return false;
		
		return std::all_of(lhs->cbegin(), lhs->cend(), 
				[&rhs](const StringMapEntry<T> & entry) {
					auto it = rhs->find(entry.first);
					if(it == rhs->cend())
						return false;
					return *(it->second) == *(entry.second);
				});
	}
};
