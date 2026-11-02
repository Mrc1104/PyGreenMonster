#include "CMsgServer.h"
#include "ClientException.h"
#include <iostream>
#include <chrono>
#include <thread>
using namespace RemexClient;

// Defeaults to multicars
CMsgDomain::CMsgDomain()
: domain("cMsg://multicast/cMsg/remex?multicastTO=5")
{}
CMsgDomain::CMsgDomain(CMsgHost host)
{
	domain = "cMsg://"
	       + host.get()
	       + "/cMsg/remex?multicastTO=5";

}

CMsgUDL::CMsgUDL(CMsgDomain domain, CMsgPassword password)
:udl( domain.get() + password.get() )
{
}

CMsgServerName::CMsgServerName()
: CMsgServerName("")
{
	constexpr unsigned _size{255};
	char tmp[_size];
	if( ::gethostname(tmp,_size) ) {
		throw ClientException(
			std::string("gethostname failed.  Must specify hostname"));
	}
	name = tmp;
	name += "_";
	name +=  std::to_string( ::getpid() );
}
CMsgServerName::CMsgServerName(std::string name)
: CMsgServerName(
	std::move(name),
	"Sender of Commands to a Remote Executer")
{}
CMsgServerName::CMsgServerName(std::string name, std::string descr)
: name(std::move(name))
, description(std::move(descr)) {}

CMsgServer::CMsgServer(CMsgServerName name,  CMsgUDL udl)
: handle{nullptr}
{
	std::cout << "Trying to connect to UDL( " << udl.get() << " )\n";
	auto status = ::cMsgConnect(
			udl.get().c_str(), name.get_name().c_str(),
			name.get_descr().c_str(), &handle);
	if( status != CMSG_OK ) throw ClientException( std::string( "Could not connect to UDL") );
  	::cMsgReceiveStart(handle);
}

CMsgServer::~CMsgServer()
{
	// Artificial delay to avoid some cMsg write failure messages
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(10ms); 
	auto status = ::cMsgDisconnect(&handle);
	if (status != ::CMSG_OK) {
		std::cerr << "cMsgDisconnect returned (";
		std::cerr << status << ") " <<  ::cMsgPerror(status) << '\n';
	}
}


