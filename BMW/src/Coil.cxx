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
	trig_.Attach(pv);
	if(isEnergyModulator()) {
		ampl_ = RFVernierAmplitude();
		wave_.Set( WaveformType::Sine );
	}
}
// For placement new
Coil::~Coil() = default;

bool Coil::isEnergyModulator() const
{
	return meta_.mod_ == Modtype::BMWEnergy;
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

bool Coil::SetTriggerState(TrigState const state)
{

	return trig_.SetAndBroadcast(state);
}


const char* Coil::GetName() const { return meta_.name_.Get(); }
int Coil::GetID()           const { return meta_.id_.Get();   }
Modtype Coil::GetModType()  const { return meta_.mod_;  }
Status Coil::GetStatus()    const { return state_.GetStatus();}

