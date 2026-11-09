#include "Status.h"
#include "Cycles.h"
#include "MetaData.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace BMW;
int main()
{
	MetaData meta;
	std::string coil;
	for(auto& data : meta.Get([](Meta const& meta) { return meta.id_.Get() == 0; })) {
		std::cout << data.id_.Get() << '\t' << data.name_.Get() << '\n';
		coil = data.GetPV();
	}

	State state(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "Coil Conn Status = " << state.GetConnStatus() << '\n';
	std::cout << "Coil Status = "      << stringify(state.GetStatus()) << '\n';

	Cycles cycle(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to Set NCycles\n";
	std::cout << "=================================\n";
	if(cycle.Set(50)) {
		std::cout << "Set Cycles succeeded\n";
	} else {
		std::cout << "Set Cycles failed\n";
	} 
}
