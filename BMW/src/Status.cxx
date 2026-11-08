#include "Status.h"
#include <string_view>
#include <string>
#include "QwEpicHandler.h"

using namespace BMW;
Status BMW::convertToStatus(int input)
{
	Status state = Status::Unknown;
	switch(static_cast<Status>( input )) {
		case Status::Off:     state = Status::Off;    break;
		case Status::Config:  state = Status::Config; break;
		case Status::Trigger: state = Status::Trigger;break;
		case Status::Test:    state = Status::Test;   break;
		case Status::Unknown: state = Status::Unknown;break;
	}
	return state;
}


State::State(std::string_view pv)
{
	Attach(pv);
}

State::~State()
{
	if(channel_) channel_->StopMonitoring(this);
	channel_ = nullptr;
}

State::State(State const& other)
: channel_(other.channel_)
{
	status_.Store(other.status_.Load());
}

void State::Attach(std::string_view pv)
{
	if(channel_) channel_->StopMonitoring(this);
	std::string suffix{};
	if(pv.find(EPIC_PV_SUFFIX) == std::string_view::npos) {
		suffix = std::string(EPIC_PV_SUFFIX);
	}
	std::string null_term_string = std::string(pv) + suffix;
	channel_ = EpicHandler::Instance().ConnectChannel(null_term_string.c_str());
	channel_->StartMonitoring(this);
}


Status State::GetStatus() const { return convertToStatus(status_.Load()); }

void State::Update(int const& data)
{
	status_.Store(data);
	// Notify
}
