#include <iostream>
#include <string>
#include <memory>
#include <sstream>
#include "Utils/MetaUtils.hpp"
#include "Value/Value.hpp"

struct C 
{
	C(int i) {};
	int operator()(int i)
	{
		return 0;
	}
};

int main()
{
	float f = 2.5f;
	Value v(true);
	std::string coisas("3123");
	//v = "asd";

	std::cout << *v.convertTo<std::string>() << std::endl;
	
	//Value v();
}