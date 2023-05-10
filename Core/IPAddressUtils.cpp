#include "pch.h"
#include "IPAddressUtils.h"

#include "Enum.h"


int64_t IPAddressUtils::parse(std::string_view ipstring)
{
	if (ipstring.empty())
		return 0;

	SOCKADDR_IN sin = {};

	// 일단 Ipv4만 대응
	sin.sin_family = (ADDRESS_FAMILY)AddressFamily::INET;
	if (ipstring.compare("localhost") == 0)
	{
		inet_pton((INT)sin.sin_family, "127.0.0.1", &sin.sin_addr.s_addr);
	}
	else if (ipstring.compare("0.0.0.0") == 0)
	{
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		inet_pton((INT)sin.sin_family, ipstring.data(), &sin.sin_addr.s_addr);
	}

	return sin.sin_addr.s_addr;
}
