#include "Coil.h"

using namespace BMW;

Coil::Coil(Meta const& meta)
: meta_(meta)
{
	auto pv = meta_.GetPV();
	state_.Attach(pv);
	wave_.Attach(pv);
	ampl_.Attach(pv);
	cycles_.Attach(pv);
	freq_.Attach(pv);
	if(isEnergyModulator()) {
		ampl_ = RFVernierAmplitude();
		wave_.Set( WaveformType::Sine );
	}
}

bool Coil::isEnergyModulator() const
{
	return meta_.mod_ == Modtype::Energy;
}

bool Coil::SetAmplitude(double const input)
{
	return ampl_.SetAndBroadcast(input);
}

bool Coil::SetWave(WaveformType const input)
{
	// Energy Amp is indifferent to wave type
	if(!isEnergyModulator() && input != wave_.Get()) {
		auto amp = ampl_.Get();
		if(input == WaveformType::Sine) ampl_ = SineAmplitude();
		else ampl_ = RampAmplitude();
		return wave_.SetAndBroadcast(input) and ampl_.SetAndBroadcast(amp);
	}
	return wave_.SetAndBroadcast(input);
}

bool Coil::SetFrequency(double const input)
{
	return freq_.SetAndBroadcast(input);
}

bool Coil::SetCycles(std::size_t const input)
{
	return cycles_.SetAndBroadcast(input);
}

/////////////////////////////////////////////////
//// EXTERN C API
/////////////////////////////////////////////////
/*
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

void AttachReceiver(BMWCoil coil, BMWReceiver receiver)
{
	auto ptr = static_cast<Coil*>(coil);
	auto rec = static_cast<Receiver<BMWMessage>*>(receiver);
	ptr->AttachReceiver(rec)
}
void SetBMWAmplitude(BMWCoil coil, double input)
{
	auto ptr = static_cast<Coil*>(coil);
	ptr->SetAmplitude(input);
}
void SetBMWWave(BMWCoil coil, WaveformType const input)
{
	auto ptr = static_cast<Coil*>(coil);
	ptr->SetWave(input);
}
void SetBMWFrequency(BMWCoil coil, double const input)
{
	auto ptr = static_cast<Coil*>(coil);
	ptr->SetFrequency(input);
}
void SetBMWCycles(BMWCoil coil, std::size_t const input)
{
	auto ptr = static_cast<Coil*>(coil);
	ptr->SetCycles(input);
}
*/
