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
	Value str("asd");
	Value::Vector vec;


	std::string coisas("3123");
	Value v1(vec);

	//vec.emplace_back(std::make_shared<const Value>(3123123));

	Value v2(vec);


	std::cout << *v1.convertTo<std::string>(std::make_shared<std::string>("3232321323")) << std::endl;

	
	//Value v();
}


