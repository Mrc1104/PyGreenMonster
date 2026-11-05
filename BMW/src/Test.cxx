#include "Test.h"
#include "Client.h"
#include <iostream>
void BMW::TEST()
{
	using namespace RemexClient;
	std::cout << "BMW::TEST Called\n";	
	Client(ClientName(std::string("test")), nullptr);
}

