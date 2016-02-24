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

std::shared_ptr<const std::string> ss(const char * c) { return std::make_shared<std::string>(c);}

int main()
{
	Value::StringMap sm;

	float f = 2.5f;
	Value v(std::string("323"));
	
	
	std::cout << v.convertTo<Value::String>() << std::endl;
	
	//Value v();
}