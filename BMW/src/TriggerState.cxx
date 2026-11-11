#include "TriggerState.h"

using namespace BMW;
TriggerState::TriggerState(std::string_view pv)
: TriggerState()
{
	Attach(pv);
}

void TriggerState::Attach(std::string_view pv)
{
	enable_.Attach(pv);	
	disable_.Attach(pv);	
}
void TriggerState::Set(TrigState input)
{
	active_ = input;
}
bool TriggerState::Broadcast()
{
	// Question:
	// Does writing a '1' to the enable PV automatically write
	// a '0' to the disable PV?
	// Not sure, will toggle both
	return (enable_.Toggle(active_) and disable_.Toggle(active_));
}

bool TriggerState::SetAndBroadcast(TrigState input)
{
	Set(input);
	return Broadcast();
}
TrigState TriggerState::Get() const { return active_; }
