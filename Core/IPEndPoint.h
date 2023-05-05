#pragma once

#include "SocketAddress.h"
#include "IPAddress.h"

class IPEndPoint
{
private:
	int64_t ip_ = 0;
	AddressFamily addressFamily_ = AddressFamily::INET;
	int port_;

public:
	bool isValid();

public:
	void setAddress(int64_t address) { ip_ = address;}
	void setPort(int port) { port_ = port; }

public:
	AddressFamily getAddressFamily() { return addressFamily_; }
	SocketAddress serialize();

public:
	std::string getIPString();
};

