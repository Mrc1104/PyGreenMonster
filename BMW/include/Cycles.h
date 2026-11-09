#pragma once
#include "QwEpicChannel.h"
#include <string_view>
namespace BMW
{
	class Cycles
	{
	public:
		// Finite: [1,510]
		// Infinite: 511
		static constexpr int lower_bound_{1};
		static constexpr int upper_bound_{511};
		static constexpr std::string_view EPIC_PV_SUFFIX{":RCRINPUT"};
		explicit Cycles(int input = lower_bound_);
		Cycles(std::string_view pv, int input = lower_bound_);
		void Attach(std::string_view pv);
		bool Set(int input);
		int Get() const;
	private:
		int cycles_{lower_bound_};
		EpicChannel* channel_{nullptr};
	};
};

