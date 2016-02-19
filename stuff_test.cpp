#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include "Value/Value.hpp"


struct A
{
	friend std::ostream & operator<<(std::ostream & os, const A & a)
	{
		os << "ola a";
		return os;
	}
};

int main()
{
	Value val("dasdasd");
	A a;
	std::cout << val.convertTo<Value::Vector>() << std::endl;
	return 0;
	//Value v();
}