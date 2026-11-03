#ifndef __CLIENT_EXCEPTION_H__
#define __CLIENT_EXCEPTION_H__
#include <stdexcept>
namespace RemexClient
{
	struct ClientException : public std::runtime_error
	{
		explicit ClientException(std::string const& msg)
		: std::runtime_error("Remex Client Error: " + msg) {}
	};
};

#endif
