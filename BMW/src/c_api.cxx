#include "c_api.h"
#include "Coil.h"
#include "MetaData.h"
#include "GlobalEnable.h"
#include "Manager.h"
#include <new>
#include <thread>
#include <chrono>
#include <cstdlib>
#include <iostream>

BMWCoilObj CreateListOfHallACoils(std::size_t* items)
{
	using namespace BMW;

	auto& meta = MetaData::Instance();
	// Loop once to get the size
	std::size_t counter{0};
	counter = 0;
	for([[maybe_unused]] auto& data : meta.GetHallA()) {
		counter++;
	}
	*items = counter;
	if(counter == 0) return nullptr;

	void* buffer = std::malloc(counter * sizeof(Coil));
	if(!buffer) {
		*items = 0;
		return nullptr;
	}

	counter = 0;
	Coil* coil_array = static_cast<Coil*>( buffer );
	// Loop twice to fill
	for(auto& data : meta.GetHallA()) {
		std::cout << "Creating coil #" << counter << '\n';
		new (&coil_array[counter++]) Coil(data);
	}
	return static_cast<BMWCoilObj>( coil_array );
}
void DestroyListOfCoils(BMWCoilObj coil, std::size_t items)
{
	using namespace BMW;
	if(coil == nullptr) return;
	auto arr = static_cast<Coil*>(coil);
	for( std::size_t i = 0; i < items; i++) {
		std::cout << "Destroying coil #" << i << '\n';
		arr[i].~Coil();
	}
	free( arr );
}

std::size_t GetCoilMemSize()
{ 
	using namespace BMW;
	return sizeof(Coil);
}

bool SetBMWAmplitude(BMWCoilObj coil, double input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetAmplitude(input);
}
bool SetBMWWave(BMWCoilObj coil, WaveformType const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetWave(input);
}
bool SetBMWFrequency(BMWCoilObj coil, double const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetFrequency(input);
}
bool SetBMWCycles(BMWCoilObj coil, int const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetCycles(input);
}

bool SetBMWTrigger(BMWCoilObj coil, TrigState const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetTriggerState(input);
}


const char* GetCoilName(BMWCoilObj coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetName();
}
int GetCoilId(BMWCoilObj coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetID();
}
Status GetStatus(BMWCoilObj coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetStatus();
}


Modtype GetModType(BMWCoilObj coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetModType();
}

bool EnableBeamMod()
{
	using namespace BMW;
	return MasterSwitch::Instance().Enable();
}
bool DisableBeamMod()
{
	using namespace BMW;
	return MasterSwitch::Instance().Disable();
}
bool isBeamModEnabled()
{
	using namespace BMW;
	return MasterSwitch::Instance().isEnabled();
}
bool BeamModStartup()
{
	// This routine makes sure that the Global switch
	// is instantiated. It waits for a set time and then
	// attempts to read the value
	using namespace BMW;
	auto& manager       = Manager::Instance();
	auto& master_switch = MasterSwitch::Instance();
	std::this_thread::sleep_for(std::chrono::milliseconds(250));
	return master_switch.isEnabled();
}
