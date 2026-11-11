#include "c_api.h"
#include "Coil.h"
#include "MetaData.h"
#include <new>
#include <cstdlib>
#include <iostream>

BMWCoil CreateListOfHallACoils(std::size_t* items)
{
	using namespace BMW;

	auto& meta = MetaData::Instance();
	// Loop once to get the size
	std::size_t counter{0};
	counter = 0;
	for(auto& data : meta.GetHallA()) {
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
	return static_cast<BMWCoil>( coil_array );
}
void DestroyCoil(BMWCoil coil, std::size_t items)
{
	using namespace BMW;
	if(coil == nullptr) return;
	auto arr = static_cast<Coil*>(coil);
	for( std::size_t i = 0; i < items; i++) {
		arr[i].~Coil();
	}
	free( arr );
}

std::size_t GetCoilMemSize()
{ 
	using namespace BMW;
	return sizeof(Coil);
}

bool SetBMWAmplitude(BMWCoil coil, double input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetAmplitude(input);
}
bool SetBMWWave(BMWCoil coil, WaveformType const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetWave(input);
}
bool SetBMWFrequency(BMWCoil coil, double const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetFrequency(input);
}
bool SetBMWCycles(BMWCoil coil, int const input)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetCycles(input);
}

const char* GetCoilName(BMWCoil coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetName();
}
int GetCoilId(BMWCoil coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetID();
}
Status GetStatus(BMWCoil coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetStatus();
}


Modtype GetModType(BMWCoil coil)
{
	using namespace BMW;
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetStatus();
}

