#include "Client.h"
#include <iostream>
#include <cstdio>

#if __cplusplus >= 202002L
#include <source_location>
	#define GET_LOCATION std::source_location::current().function_name()
#else 
	#define GET_LOCATION __FUNCTION__
#endif
using namespace RemexClient;

Client::Client(ClientName name,  void* handle)
: name(std::move(name))
, handle(handle)
{}

void Client::PrintStatus(::remexErrCode code)
{
	std::cerr << GET_LOCATION << "\n\t";
	switch(code)
	{
		case ::remexErrCode::REMEX_ERRCODE_SUCCESS:
			std::cerr << "Success (" << code << ")\n";
			break;

		case ::remexErrCode::REMEX_ERRCODE_DLOPEN_FAILURE:
			std::cerr << "Error calling dlopen(...) to load current symbols (" << code << ")\n";
			break;

		case ::remexErrCode::REMEX_ERRCODE_DLCLOSE_FAILURE:
			std::cerr << "Error calling dlclose() to close symbol handle (" << code << ")\n";
			break;

		case ::remexErrCode::REMEX_ERRCODE_FUNC_NOT_FOUND:
			std::cerr << "Error calling dlsym(..).  Function not found. (" << code << ")\n";
			break;

		case ::remexErrCode::REMEX_ERRCODE_VAR_NOT_FOUND:
			std::cerr << "Error calling dlsym(..).  Variable not found. (" << code << ")\n";
			break;

		default:
			std::cerr << "Unknown Error Code (" << code << ")\n";
			break;
	}
	return;
}


