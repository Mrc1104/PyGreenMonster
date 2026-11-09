#include "c_api.h"

BMWCoil CreateListOfHallACoils(std::size_t* items)
{
	// Loop once to get the size
	std::size_t counter{0};
	counter = 0;
	for(auto& data : meta.Get([](BMW::Meta const& meta) { return meta.hall_ == BMW::Hall::A; })) {
		if( Modtype::Empty == data.mod_ ) continue;
		counter++;
	}
	*items = counter;
	if(counter == 0) return nullptr;

	Coil* coils = new Coil[counter];
	counter = 0;

	// Loop twice to fill
	for(auto& data : meta.Get([](BMW::Meta const& meta) { return meta.hall_ == BMW::Hall::A; })) {
		if( Modtype::Empty == data.mod_ ) continue;
		coils[counter++] = new Coil(data);	
	}
	return static_cast<BMWCoil>( coils );
}
DestroyCoil(BMWCoil* coil, std::size_t items)
{
	if(coil == nullptr) return;
	auto ptr = static_cast<Coil*>(coil);
	delete[] ptr;
}

std::size_t GetCoilMemSize() { return sizeof(Coil); }

bool SetBMWAmplitude(BMWCoil coil, double input)
{
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetAmplitude(input);
}
return SetBMWWave(BMWCoil coil, WaveformType const input)
{
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetWave(input);
}
bool SetBMWFrequency(BMWCoil coil, double const input)
{
	auto ptr = static_cast<Coil*>(coil);
	return->SetFrequency(input);
}
bool SetBMWCycles(BMWCoil coil, int const input)
{
	auto ptr = static_cast<Coil*>(coil);
	return ptr->SetCycles(input);
}

const char( GetCoilName(BMWCoil coil)
{
	auto ptr = static_cast<Coil*>(coil);
	return ptr->meta_.name_.Get();
}
int GetCoilId(BMWCoil coil)
{
	auto ptr = static_cast<Coil*>(coil);
	return ptr->meta_.id_.Get();
}
Status GetStatus(BMWCoil coil)
{
	auto ptr = static_cast<Coil*>(coil);
	return ptr->GetStatus();
}

