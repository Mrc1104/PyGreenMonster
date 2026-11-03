#include "ClientTypes.h"
#include <unistd.h>
#include "ClientException.h"

using namespace RemexClient;
ClientName::ClientName()
{
	constexpr unsigned _size{255};
	char tmp[_size];
	if( ::gethostname(tmp,_size) ) {
		throw ClientException(
			std::string("gethostname failed.  Must specify hostname"));
	}
	name = tmp;
}
ClientName::ClientName(std::string name)
: name(std::move(name))
{}

