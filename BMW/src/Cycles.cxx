#include "Cycles.h"
#include "QwEpicHandler.h"
using namespace BMW;


Cycles::Cycles(int input)
: cycles_(input) { }
Cycles::Cycles(std::string_view pv, int input)
: Cycles(input)
{
	Attach(pv);
}

void Cycles::Attach(std::string_view pv)
{
	std::string suffix{};
	if(pv.find(EPIC_PV_SUFFIX) == std::string_view::npos) {
		suffix = std::string(EPIC_PV_SUFFIX);
	}
	std::string null_term_string = std::string(pv) + suffix;
	channel_ = EpicHandler::Instance().ConnectChannel(null_term_string.c_str());
}

bool Cycles::Set(int input)
{
	cycles_ = std::clamp(input, lower_bound_, upper_bound_);
	if(!channel_) {
		return false;
	}
	auto fut = channel_->PutAsync(cycles_);
	try {
		fut.get();
	} catch(std::runtime_error const& e) {
		std::cerr << "Error Setting Cycles: " << e.what() << '\n';
		return false;
	}
	// TODO: Send to Remex
	return true;
}

int Cycles::Get() const { return cycles_; }

