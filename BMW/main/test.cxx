#include "Test.h"
#include "Waveforms.h"
#include <iostream>

int main()
{
	using namespace BMW;
	TEST();
	Frequency freq{9};
	std::cout << freq.Get() << '\n';
	freq.Set(249);
	std::cout << freq.Get() << '\n';

	RampAmplitude ramp;
	std::cout << ramp.Get() << '\n';
	SineAmplitude sine(-0.380);
	std::cout << sine.Get() << '\n';
	RFVernierAmplitude rfvern(0.156);
	std::cout << rfvern.Get() << '\n';

	
	return 0;
};
