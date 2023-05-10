#include "pch.h"
#include "IPEndPoint.h"

IPEndPoint::IPEndPoint(int64_t ip, int port, AddressFamily addressFamily)
	: ip_(ip)
	, port_(port)
	, addressFamily_(addressFamily)
{
	
}

bool IPEndPoint::isValid()
{
	if (ip_ < 0)
		return false;

	if (ip_ > std::numeric_limits<uint32_t>::max())
		return false;

	return true;
}

SocketAddress IPEndPoint::serialize() const
{
	return SocketAddress(ip_, addressFamily_, port_);
}

std::string IPEndPoint::getIPString()
{
	if (isValid() == false)
		return "";

	char ip[17] = { 0, };
	inet_ntop(static_cast<INT>(addressFamily_), &ip_, ip, 17);

	return ip;
}
