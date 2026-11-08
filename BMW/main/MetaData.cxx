#include "MetaData.h"
#include <iostream>


int main()
{
	BMW::MetaData meta;
	std::cout << "sizeof(MetaData) == " << sizeof(BMW::MetaData) << " [bytes]\n";
	std::cout << "Hall A:\n";
	for(auto& data : meta.Get([](BMW::Meta const& meta) { return meta.hall_ == BMW::Hall::A; })) {
		std::cout << data.id_.Get() << '\t' << data.name_.Get() << '\n';
	}

	std::cout << "\n=====================================\n\n";

	std::cout << "Hall C:\n";
	for(auto& data : meta.Get([](BMW::Meta const& meta) { return meta.hall_ == BMW::Hall::C; })) {
		std::cout << data.id_.Get() << '\t' << data.name_.Get() << '\n';
	}
	return 0;
}
