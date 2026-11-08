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
