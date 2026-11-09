#pragma once

namespace BMW {
	class Frequency
	{
	public:
		// Can the frequency be floating-point?
		// Or do they have to be integral values?
		using dataType = unsigned;
		Frequency();
		explicit Frequency(dataType input);
		void Set(dataType input);
		dataType Get() const;
		static constexpr dataType lower_bound_{10};
		static constexpr dataType upper_bound_{250};
	private:
		dataType value_{lower_bound_}; // Units: [Hz]
	};
};
