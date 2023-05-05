#pragma once

#include "Enum.h"

class IPAddress final
{
private:
	bool isInvalid_ = false;
	int64_t address_ = 0;
	AddressFamily addressFamily_ = AddressFamily::UNSPEC;

public:
	IPAddress() = delete;
	IPAddress(int64_t address);
};

