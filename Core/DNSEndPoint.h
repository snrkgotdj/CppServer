#pragma once

#include "IPEndPoint.h"

class DNSEndPoint final : public IPEndPoint
{
private:
	std::string host_;

public:
	DNSEndPoint() = delete;
	DNSEndPoint(std::string_view host, int32_t port);

};

