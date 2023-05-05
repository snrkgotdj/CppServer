#include "pch.h"
#include "IPAddress.h"

IPAddress::IPAddress(int64_t address)
	:address_(address)
	,addressFamily_(AddressFamily::INET)
{
	isInvalid_ = address < 0 || address > std::numeric_limits<uint32_t>::max();
}
