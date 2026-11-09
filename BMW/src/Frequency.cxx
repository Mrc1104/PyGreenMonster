#include "Frequency.h"
#include <algorithm>
using namespace BMW;


BMW::Frequency::Frequency() = default;
BMW::Frequency::Frequency(dataType input)
{
	Set(input);
}

void BMW::Frequency::Set(dataType input) {

	value_ = std::clamp(input, lower_bound_, upper_bound_);
}
BMW::Frequency::dataType BMW::Frequency::Get() const { return value_; }


