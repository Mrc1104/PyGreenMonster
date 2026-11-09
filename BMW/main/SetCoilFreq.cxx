
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

const char* MASTER_SWITCH = "BEAMMODSWITCH";
constexpr unsigned conn_wait = 500;

using namespace BMW;


void usage(char const* exe)
{

	MetaData meta;

	std::cout << "Usage:\n" << exe << " <Coil> <Cycles>\n";
	std::cout << "Example: ./build/BMW/setcoilfreq MHF1C08H 205\n";
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
int main(int argc, char** argv)
{
	if(argc != 3) {
		usage(argv[0]);
		return 0;
	}
	MetaData meta;
	std::string coil;
	char const* targ_coil = argv[1];
	int targ = std::atoi(argv[2]);
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


	print_section_header("Modify Coil Cycles");
	Frequency freq(coil);
	std::this_thread::sleep_for(std::chrono::milliseconds(conn_wait));
	auto curr_value = freq.GetRemote().value_or(-1);
	std::cout << "Current Value = " <<  curr_value << '\n';

	if(freq.SetAndBroadcast(targ)) {
		std::cout << "Set Frequency succeeded\n";
		std::cout << "New Value = " << freq.GetRemote().value_or(-1) << '\n';
	} else {
		std::cout << "Set Frequency failed\n";
	} 

	freq.SetAndBroadcast(curr_value);
	print_section_header("Disable Master Switch");
	master->PutAsync(0).get();

	return 0;
}
