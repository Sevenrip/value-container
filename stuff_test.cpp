#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include "Utils/MetaUtils.hpp"
#include "Value/Value.hpp"

struct C 
{
	int operator()(int i)
	{
		return 0;
	}
};

int main()
{
	Value v("asdasd");
	
	//v = "asd";
	
	std::cout << *v.convertTo<std::string>() << std::endl;
	//Value v();
}