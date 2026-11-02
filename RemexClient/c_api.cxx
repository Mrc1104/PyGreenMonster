#include "c_api.h"
#include <string>
#include <cMsg.h>
#include "remexLib.h" // enum varReturnType, struct varReturnVal
#include "CMsgServer.h"
#include "ClientTypes.h"
#include "Client.h"

CMsgServer CreateCMsgServer(char const *host, char const *pass)
{
	auto server_host = (host == nullptr)
	   		   ? RemexClient::CMsgHost{}
	       		   : RemexClient::CMsgHost{std::string(host)};
	
	auto server_psswrd = (pass == nullptr)
	       	             ? RemexClient::CMsgPassword{}
	       	             : RemexClient::CMsgPassword{std::string(pass)};

	RemexClient::CMsgUDL udl(
			RemexClient::CMsgDomain(std::move(server_host)),
			std::move(server_psswrd));

	return new RemexClient::CMsgServer(
			RemexClient::CMsgServerName{}, std::move(udl) );
}
void DestoryCMsgServer(CMsgServer server)
{
	delete static_cast<RemexClient::CMsgServer*>(server);
}

Client CreateClient(char const* remex_host, CMsgServer server)
{
	auto remex_hostname = remex_host 
			? RemexClient::ClientName(std::string(remex_host))
			: RemexClient::ClientName();
	auto server_ptr = static_cast<RemexClient::CMsgServer*>(server);
	return new RemexClient::Client(std::move(remex_hostname), server_ptr->get_handle());
}

void DestoryClient(Client client)
{
	delete static_cast<RemexClient::Client*>(client);
}
// I probably should look into a way to propogate the error upwards... something like
// struct {
//	varReturnVal
//	bool success
// };
// and set the success boolean based on the existence of 
// the std::optional value. Then python can check that status
struct RemexReturnVal SendCommand(Client client, char const* cmd, enum varReturnType rtype)
{
	auto remex = static_cast<RemexClient::Client*>(client);
	auto command = RemexClient::ClientCommand(std::string(cmd));
	struct RemexReturnVal val{false, 0};
	switch( rtype ) {
	case rVOID:
		if( auto opt = remex->Execute<rVOID>(command); opt.has_value()) {
			val.success = true;
		}
		break;
	case rFLOAT:
		if( auto opt = remex->Execute<rFLOAT>(command); opt.has_value()) {
			val.success = true;
			val.val.rFloat = opt.value();
		}
		break;
	case rDOUBLE:
		if( auto opt = remex->Execute<rDOUBLE>(command); opt.has_value()) {
			val.success = true;
			val.val.rDouble= opt.value();
		}
		break;
	case rINT32:
		if( auto opt = remex->Execute<rINT32>(command); opt.has_value()) {
			val.success = true;
			val.val.rInt32= opt.value();
		}
		break;
    	case rINT16:
		if( auto opt = remex->Execute<rINT16>(command); opt.has_value()) {
			val.success = true;
			val.val.rInt16= opt.value();
		}
		break;
    	case rUINT32:
		if( auto opt = remex->Execute<rUINT32>(command); opt.has_value()) {
			val.success = true;
			val.val.rUint32= opt.value();
		}
		break;
    	case rUINT16:
		if( auto opt = remex->Execute<rUINT16>(command); opt.has_value()) {
			val.success = true;
			val.val.rUint16= opt.value();
		}
		break;
	default:
		std::cerr << "Unknown ::varReturnType ( " << rtype << " ) given\n";
		break;
	}
	return val;
}
