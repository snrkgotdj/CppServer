#pragma once

#include "SocketAddress.h"

class IPEndPoint
{
private:
	int64_t ip_ = 0;
	AddressFamily addressFamily_ = AddressFamily::INET;
	int port_ = 0;

public:
	bool isValid();

public:
	void setIP(int64_t address) { ip_ = address;}
	void setPort(int port) { port_ = port; }

public:
	SocketAddress serialize() const;
	AddressFamily getAddressFamily() { return addressFamily_; }
	std::string getIPString();

public:
	IPEndPoint() = default;
	IPEndPoint(int64_t ip, int port, AddressFamily addressFamily = AddressFamily::INET);
};

