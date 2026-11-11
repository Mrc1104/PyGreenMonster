#include "Status.h"
#include "Cycles.h"
#include "MetaData.h"
#include "Amplitude.h"
#include "Frequency.h"
#include "Waveforms.h"
#include "TriggerState.h"
#include <string>
#include <iostream>
#include <thread>
#include <chrono>

const char* MASTER_SWITCH = "BEAMMODSWITCH";
constexpr unsigned conn_wait = 500;

using namespace BMW;


void usage(char const* exe)
{

	auto& meta = MetaData::Instance();

	std::cout << "Usage:\n" << exe << " <Coil>\n";
	std::cout << "Example: ./build/BMW/setcoiltrigstate MHF1C08H\n";
	std::cout << "\n[coils:";
	for(auto& data : meta.Get([](BMW::Meta const& meta) { return meta.hall_ == BMW::Hall::A; })) {
		std::cout << ' ' << data.name_.Get();
	}
	std::cout << "]\n";

	
};


void print_section_header(const char* text)
{
  int box_width = 58;
  int text_len = strlen(text);

  int left_pad = (box_width - text_len) / 2;
  int right_pad = box_width - text_len - left_pad;

  printf("############################################################\n");
  printf("#%*s%s%*s#\n", left_pad, "", text, right_pad, "");
  printf("############################################################\n");
}

Ampl GetAmplitude(int wave = 0) {
	if(wave == 0) return SineAmplitude();
	if(wave == 1) return RampAmplitude();
	else return RFVernierAmplitude();
}
int main(int argc, char** argv)
{
	if(argc != 2) {
		usage(argv[0]);
		return 0;
	}
	auto& meta = MetaData::Instance();
	std::string coil;
	char const* targ_coil = argv[1];
	for(auto& data : meta.Get([targ_coil](Meta const& meta) { return meta.hall_ == BMW::Hall::A && std::string_view(meta.name_.Get()) == std::string_view(targ_coil); })) {
		std::cout << "Requested Coil: " << data.name_.Get() << " (id: " << data.id_.Get() << ")\n";
		coil = data.GetPV();
	}

	print_section_header("Enable Master Switch");
	auto master = EpicHandler::Instance().ConnectChannel(MASTER_SWITCH);
	std::this_thread::sleep_for(std::chrono::milliseconds(conn_wait));
	master->PutAsync(1).get();


	print_section_header("Check Coil Status");
	State state(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(conn_wait));
	std::cout << "Coil Conn Status = " << state.GetConnStatus() << '\n';
	std::cout << "Coil Status = "      << stringify(state.GetStatus()) << '\n';


	print_section_header("Enabling Trigger");
	TriggerState trig(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(conn_wait));
	if(trig.SetAndBroadcast(TrigState::BMWEnable)) {
		std::cout << "Enabling Trigger succeeded\n";
	} else {
		std::cout << "Enabling Trigger failed\n";
	} 

	do {	
		std::cout << "Press 'q' to Disable trigger and quit...\n";
	} while( getchar() != 'q' );


	print_section_header("Disable Master Switch");
	master->PutAsync(0).get();

	return 0;
}
