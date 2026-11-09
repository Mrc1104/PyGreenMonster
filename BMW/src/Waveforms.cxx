#include "Waveforms.h"
using namespace BMW;
Waveform::Waveform(std::string_view pv)
: Waveform()
{
	Attach(pv);
}

void Waveform::Attach(std::string_view pv)
{
	sine_.Attach(pv);	
	ramp_.Attach(pv);	
}
void Waveform::Set(WaveformType input)
{
	active_ = input;
}
bool Waveform::Broadcast()
{
	return (sine_.Toggle(active_) and ramp_.Toggle(active_));
}

bool Waveform::SetAndBroadcast(WaveformType input)
{
	Set(input);
	return Broadcast();
}
WaveformType Waveform::Get() const { return active_; }
