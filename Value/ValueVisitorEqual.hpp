#pragma once

#include <ostream>
#include <memory>
#include <type_traits>
#include <string>
#include "../Utils/Utils.hpp"

struct ValueVisitorEqual
{
    template <typename T, typename U, class = typename std::enable_if<!std::is_same<T,U>::value>::type>
    bool operator()(const T & lhs, const U & rhs) const
    {
        return false;
    }
	
	template <typename T>
    bool operator()(const T & lhs, const T & rhs) const
    {
        return lhs == rhs;
    }
	
	template <typename T>
    bool operator()(const std::shared_ptr<T> & lhs, const std::shared_ptr<T> & rhs) const
    {
        return this->operator()(*lhs,*rhs);
    }
	
	template <typename T, class = typename std::enable_if<std::is_arithmetic<T>::value>::type>
    bool operator()(const std::vector<T>& lhs, const std::vector<T> rhs) const
    {
		if(lhs.size() != rhs.size())
			return false;
		
        return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), 
				[this](const T & lhs, const T & rhs) {
					return this->operator()(lhs, rhs);
				});
    }

	template <typename K, typename V>
	bool operator()(const std::unordered_map<K,V> & lhs, const std::unordered_map<K,V> & rhs)
	{
		if(lhs.size() != rhs.size())
			return false;
		
		return std::all_of(lhs.cbegin(), lhs.cend(), 
				[&rhs, this](const std::pair<K,V> & entry) {
					auto it = rhs.find(entry.first);
					if(it == rhs.cend())
						return false;
					return this->operator()(it->second, entry.second);
				});
	}
};
