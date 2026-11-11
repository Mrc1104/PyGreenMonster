#include "c_api.h"
#include "Coil.h"
int main()
{
	std::size_t items = 0;
	auto coils = CreateListOfHallACoils(&items);

	sleep(1);

	std::cout << '\n';
	std::size_t size = GetCoilMemSize();
	std::cout << "Collected " << items << " coils\n";
	std::cout << "Each Coil has size of: " << size << '\n';
	std::cout << "Setting Amplitude: " << SetBMWAmplitude(coils, 50) << '\n';
	std::cout << "Setting Wave: " <<      SetBMWWave(coils, Ramp) << '\n';
	std::cout << "Setting Frequency: " << SetBMWFrequency(coils, 105) << '\n';
	std::cout << "setting Cycles: " <<    SetBMWCycles(coils, 99) << '\n';
	std::cout << "Enabling Trigger: " <<  SetBMWTrigger(coils, BMWEnable) << '\n';
	std::cout << "Disabling Trigger: " <<  SetBMWTrigger(coils, BMWDisable) << '\n';

	std::cout << "Printing out names and IDs: " << '\n';
	auto coil_arr = static_cast<BMW::Coil*>( coils );
	for(std::size_t i = 0; i < items; i++) {
		auto& c = coil_arr[i];
		std::cout << i << ")";
		std::cout << "\n\tName = " << GetCoilName(&c);
		std::cout << "\n\tID = " << GetCoilId(&c);
		std::cout << "\n\tModType = " << GetModType(&c);
	}
	std::cout << '\n';

	DestroyListOfCoils(coils, items);
};
