#include "CMsgMessages.h"

using namespace RemexClient;

std::string CMsgMessage::GetSubject() const
{
	char const* subject;
      	::cMsgGetSubject(msg, &subject);
	return std::string(subject);
}
std::string CMsgMessage::GetType() const
{
	char const* type;
	::cMsgGetType(msg, &type);
	return std::string(type);
}

int CMsgMessage::GetErrorCode() const
{
	int error;
	::cMsgGetInt32(msg, "error_code", &error);
	return error;
}

void CMsgMessage::RedirectOutput() const
{

	char const* msg_buf[REDIRECT_MAX_SIZE];
	int status = ::cMsgGetString(msg,"redirect_output",static_cast<char const**>( &msg_buf[0]) );
	if(status == ::CMSG_OK)
	{
		std::cout << "-- Redirected Output --\n";
		std::cout << msg_buf << '\n';
		std::cout << "-- Redirected Output --\n";
	}
	else
	{
		// printf("-- No Redirected Output --\n");
	}
}
