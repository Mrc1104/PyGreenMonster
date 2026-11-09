#include "Frequency.h"
#include "QwEpicHandler.h"
#include <algorithm>
using namespace BMW;


Frequency::Frequency(dataType input)
: value_{input} { }
Frequency::Frequency(std::string_view pv, dataType input )
: Frequency(input)
{
	Attach(pv);
}

void Frequency::Set(dataType input) {

	value_ = std::clamp(input, lower_bound_, upper_bound_);
}

bool Frequency::SetAndBroadcast(dataType input)
{
	Set(input);
	return Broadcast();
}
bool Frequency::Broadcast()
{
	if(!channel_) {
		return false;
	}
	auto fut = channel_->PutAsync(value_);
	try {
		fut.get();
	} catch(std::runtime_error const& e) {
		std::cerr << "Error Setting Frequency: " << e.what() << '\n';
		return false;
	}
	// TODO: Send to Remex
	return true;
}

Frequency::dataType BMW::Frequency::Get() const { return value_; }

void Frequency::Attach(std::string_view pv)
{
	std::string suffix{};
	if(pv.find(EPIC_PV_SUFFIX) == std::string_view::npos) {
		suffix = std::string(EPIC_PV_SUFFIX);
	}
	std::string null_term_string = std::string(pv) + suffix;
	channel_ = EpicHandler::Instance().ConnectChannel(null_term_string.c_str());
}
