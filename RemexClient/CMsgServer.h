#ifndef __CMSG_SERVER_H__
#define __CMSG_SERVER_H__
#include <cMsg.h>
#include <unistd.h>
#include <string>
namespace RemexClient
{
	class CMsgPassword
	{
		std::string password;
	public:
		CMsgPassword()
		: CMsgPassword( std::string(std::getenv("REMEX_CMSG_PASSWORD")) )
		{}
		explicit CMsgPassword(std::string password)
		: password( std::string("&cmsgpassword=") + password) {}
		std::string const& get() const { return password; }
	};
	// this is my SeverName
	class CMsgHost
	{
		std::string name;
	public:
		CMsgHost()
		: CMsgHost( std::string(std::getenv("REMEX_CMSG_HOST")) )
		{}
		explicit CMsgHost(std::string name)
		: name( std::move(name) ) {}
		std::string const& get() const { return name; }	
	};
	class CMsgDomain
	{
		std::string domain;
	public:
		CMsgDomain();
		explicit CMsgDomain(CMsgHost host);
		std::string const& get() const { return domain; }
	};
	class CMsgUDL
	{
		std::string udl;
	public:
		CMsgUDL(CMsgDomain domain = CMsgDomain(),
			CMsgPassword password = CMsgPassword());
		std::string const& get() const { return udl; }
	};
	class CMsgServerName
	{
		std::string name;
		std::string description;
	public:
		CMsgServerName();
		explicit CMsgServerName(std::string name);
		CMsgServerName(std::string name, std::string descr);
		std::string const& get_name() const { return name; }
		std::string const& get_descr() const { return description; }
	};


	class CMsgServer
	{
		typedef void* ConnHandle;
		ConnHandle handle;
	public:
		CMsgServer(CMsgServerName name, CMsgUDL udl);
		~CMsgServer();
		ConnHandle const get_handle() const { return handle; }
	};


}; // namespace RemexClient
#endif
