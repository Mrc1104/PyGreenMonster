#include "Status.h"
#include "Cycles.h"
#include "MetaData.h"
#include "Amplitude.h"
#include "Frequency.h"
#include "Waveforms.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace BMW;
int main()
{
	MetaData meta;
	std::string coil;
	for(auto& data : meta.Get([](Meta const& meta) { return meta.id_.Get() == 0; })) {
		std::cout << data.id_.Get() << '\t' << data.name_.Get() << '\n';
		coil = data.GetPV();
	}

	State state(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "Coil Conn Status = " << state.GetConnStatus() << '\n';
	std::cout << "Coil Status = "      << stringify(state.GetStatus()) << '\n';

	Cycles cycle(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to Set NCycles\n";
	std::cout << "=================================\n";
	if(cycle.SetAndBroadcast(50)) {
		std::cout << "Set Cycles succeeded\n";
	} else {
		std::cout << "Set Cycles failed\n";
	} 
/*
	std::cout << '\n';
	Frequency freq(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to Set Frequency\n";
	std::cout << "=================================\n";
	if(freq.SetAndBroadcast(250)) {
		std::cout << "Set Freq succeeded\n";
	} else {
		std::cout << "Set Freq failed\n";
	} 
	
	std::cout << '\n';
	Ampl ampl{RampAmplitude(coil)};
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to Set Ramp Amplitude\n";
	std::cout << "=================================\n";
	if(freq.SetAndBroadcast(250)) {
		std::cout << "Set Ampl succeeded\n";
	} else {
		std::cout << "Set Ampl failed\n";
	} 

	std::cout << '\n';
	ampl = SineAmplitude(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to Set Sine Amplitude\n";
	std::cout << "=================================\n";
	if(freq.SetAndBroadcast(250)) {
		std::cout << "Set Ampl succeeded\n";
	} else {
		std::cout << "Set Ampl failed\n";
	} 

	std::cout << '\n';
	ampl = RFVernierAmplitude(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to Set RF Amplitude\n";
	std::cout << "=================================\n";
	if(freq.SetAndBroadcast(250)) {
		std::cout << "Set Ampl succeeded\n";
	} else {
		std::cout << "Set Ampl failed\n";
	} 

	std::cout << '\n';
	Waveform waveform(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	std::cout << "\nTrying to set sinewave\n";
	std::cout << "=================================\n";
	if(waveform.SetAndBroadcast(WaveformType::Sine)) {
		std::cout << "Set Ampl succeeded\n";
	} else {
		std::cout << "Set Ampl failed\n";
	} 

	std::cout << "\nTrying to set rampwave\n";
	std::cout << "=================================\n";
	if(waveform.SetAndBroadcast(WaveformType::Ramp)) {
		std::cout << "Set Ampl succeeded\n";
	} else {
		std::cout << "Set Ampl failed\n";
	} 
	*/
	return 0;
}
