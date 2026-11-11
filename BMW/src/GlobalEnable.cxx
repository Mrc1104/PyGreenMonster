#include "GlobalEnable.h"
#include "QwEpicHandler.h"
#include "QwEpicChannel.h"
#include <thread>
#include <chrono>

using namespace BMW;
char const* MasterSwitch::EPIC_PV{"BEAMMODSWITCH"};
MasterSwitch::MasterSwitch()
{
	channel_ = EpicHandler::Instance().ConnectChannel(EPIC_PV);
	channel_->StartMonitoring(this);
}

MasterSwitch::~MasterSwitch()
{
	channel_->StopMonitoring(this);
}

MasterSwitch& MasterSwitch::Instance()
{
	static MasterSwitch instance;
	return instance;
}

bool MasterSwitch::Enable()
{
	auto fut = channel_->PutAsync(MasterSwitch::ENABLE);
	try {
		fut.get();
	} catch(std::runtime_error const& e) {
		std::cerr << "Error BeamMod Enable: " << e.what() << '\n';
		return false;
	}
	return true;
}
bool MasterSwitch::Disable()
{
	auto fut = channel_->PutAsync(MasterSwitch::DISABLE);
	try {
		fut.get();
	} catch(std::runtime_error const& e) {
		std::cerr << "Error BeamMod Disable: " << e.what() << '\n';
		return false;
	}
	return true;
}

bool MasterSwitch::isEnabled() const { return status_.Load() == MasterSwitch::ENABLE; }

void MasterSwitch::Update(int const& data)
{
	status_.Store(data);
}

