#include "Coil.h"

using namespace BMW;

Coil::Coil(Meta const& meta)
: meta_(meta)
{
	state_.Attach(meta_.name_.Get());
	if(isEnergyModulator()) {
		ampl_ = RFVernierAmplitude();
		wave_type_ = WaveformType::Sine;
	}
}

bool Coil::isEnergyModulator() const
{
	return meta_.mod_ == Modtype::Energy;
}

void Coil::SetAmplitude(double const input)
{
	ampl_.Set(input);
}

void Coil::SetWave(WaveformType const input)
{
	if(wave_type_ == input) return;
	if(isEnergyModulator()) return; // Energy mod is always sinusoidal

	wave_type_ = input; // TODO: Make wave_type_ a struct with a set operation
	auto amp = ampl_.Get();
	if(wave_type_ == WaveformType::Sine)
		ampl_ = SineAmplitude(amp);
	else 
		ampl_ = RampAmplitude(amp);
}

void Coil::SetFrequency(double const input)
{
	freq_.Set(input);
}

void Coil::SetCycles(std::size_t const input)
{
	cycles_.Set(input);
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
