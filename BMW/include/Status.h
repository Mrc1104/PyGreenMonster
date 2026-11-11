#pragma once
#include "QwEpicChannel.h"
#include "QwAtomicEpicTypes.h"
#include "QwObserver.h"
#include <string_view>
#include <string>
enum class Status : int
{
	Off = 0,
	Config,
	Trigger,
	Test,
	Unknown
};
namespace BMW
{
	Status convertToStatus(int input);
	std::string stringify(Status status);

	class State : public EPICSObserver<int>
	{
		EpicChannel* channel_{nullptr};
		AtomicEpicsType<int> status_{static_cast<int>(Status::Off)};
	public:
		static constexpr std::string_view EPIC_PV_SUFFIX{":WAVESTATE"};
		State() = default;
		State(std::string_view pv);
		~State();
		State(State const& other);
		void Attach(std::string_view pv);
		Status GetStatus() const;
		bool GetConnStatus() const;

		// No-op
		void Update(int const& data) override;

	};


};
