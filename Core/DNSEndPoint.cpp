#include "pch.h"
#include "DNSEndPoint.h"

DNSEndPoint::DNSEndPoint(std::string_view host, int32_t port)
	:host_(host)
{
	addrinfo hints;
	memset(&hints, 0x00, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;

	addrinfo* pServiceResult;
	int errorCode = getaddrinfo(host_.data(), nullptr, &hints, &pServiceResult);
	if (errorCode != 0)
	{
		PrintLastError();
		return;
	}

	while (nullptr != pServiceResult)
	{
		sockaddr_in* pAddrin = reinterpret_cast<sockaddr_in*>(pServiceResult->ai_addr);
		if (pAddrin == nullptr)
		{
			pServiceResult = pServiceResult->ai_next;
			continue;
		}

		if (pAddrin->sin_family != AF_INET)
		{
			pServiceResult = pServiceResult->ai_next;
			continue;
		}

		setAddress(pAddrin->sin_addr.s_addr);
		setPort(port);
		break;
	}

}
