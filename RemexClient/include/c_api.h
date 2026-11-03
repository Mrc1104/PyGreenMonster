#ifndef __REMEX_C_API_H__
#define __REMEX_C_API_H__
#include "remexLib.h" // enum varReturnType
extern "C"
{
	typedef void* CMsgServer;
	CMsgServer CreateCMsgServer(char const *host, char const *pass);
	void DestoryCMsgServer(CMsgServer server);

	typedef void* Client;
	Client CreateClient(char const* remex_host, CMsgServer server);
	void DestoryClient(Client client);
	struct RemexReturnVal
	{
		bool success;
		struct ::varReturnVal val;
	};
	struct RemexReturnVal SendCommand(Client client, char const* cmd, enum varReturnType rtype);
	
};


#endif
