#pragma once

#include <ostream>
#include <memory>
#include <type_traits>
#include <unordered_map>
#include <string>
#include <sstream>
#include "../Utils/Utils.hpp"


class ValueVisitorPrinter
{
public:

    explicit ValueVisitorPrinter(std::stringstream  & stream)
        : _stream(stream) {}
    ValueVisitorPrinter& operator=(ValueVisitorPrinter const&) = delete;
	
	
    template <typename T>
    void operator()(const T & t ) const
    {
        _stream << t;
    }
	
	
    void operator()(const std::shared_ptr<const std::string> & s ) const
    {
        _stream << *s;
    }

	template <typename T>
    void operator()(const std::shared_ptr<std::vector<T>> & t) const
    {
		_stream << "[ " ;
		for(const auto & el : *t) {
			this->operator()(el);
		}
		_stream << " ]" ;
    }
	
	template <typename T>
	void operator()(const std::shared_ptr<StringMap<T>> & map)
	{
		_stream << "{  " ;
		for(const auto & pair : *map) {
			_stream << "{" ;
			this->operator()(pair.first);
			this->operator()(pair.second);
			_stream << "}" ;
		}
		_stream << "  }" ;
		
	}
	
private:
	int _depth;
	std::stringstream  & _stream;
};
