#include "MetaData.h"
#include <iostream>


int main()
{
	auto& meta = BMW::MetaData::Instance();
	std::cout << "sizeof(MetaData) == " << sizeof(BMW::MetaData) << " [bytes]\n";
	std::cout << "Hall A:\n";
	for(auto& data : meta.GetHallA()) {
		std::cout << data.id_.Get() << '\t' << data.name_.Get() << '\n';
	}

	std::cout << "\n=====================================\n\n";

	std::cout << "Hall C:\n";
	for(auto& data : meta.GetHallC()) {
		std::cout << data.id_.Get() << '\t' << data.name_.Get() << '\n';
	}
	return 0;
}
