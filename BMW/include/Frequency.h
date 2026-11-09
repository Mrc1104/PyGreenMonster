#pragma once

#include "QwEpicChannel.h"
#include <string_view>
#include <optional>
namespace BMW {
	class Frequency
	{
	public:
		// Can the frequency be floating-point?
		// Or do they have to be integral values?
		using dataType = int;
		static constexpr dataType lower_bound_{10};
		static constexpr dataType upper_bound_{250};
		static constexpr std::string_view EPIC_PV_SUFFIX{":FREQUENCY"};
		explicit Frequency(dataType input = lower_bound_);
		Frequency(std::string_view pv, dataType input = lower_bound_);
		void Attach(std::string_view pv);
		void Set(dataType input);
		bool SetAndBroadcast(dataType input);
		bool Broadcast();
		dataType Get() const;
		std::optional<int> GetRemote() const;
	private:
		dataType value_{lower_bound_}; // Units: [Hz]
		EpicChannel* channel_{nullptr};
	};
};
