#pragma once

#include "Enum.h"

class SocketAddress final
{
private:
	char* buffer_ = nullptr;
	size_t bufferMaxSize_ = 0;
	size_t bufferLength_ = 0;

private:
	void initBuffer(AddressFamily addressFamily);
	void init(int64_t ip, AddressFamily addressFamily, int32_t port);
	void destroy();

public:
	char* getBuffer() const { return buffer_; }
	size_t getBufferLength() const { return bufferLength_; }

public:
	SocketAddress();
	SocketAddress(int64_t ip, AddressFamily addressFamily,  int32_t port);
	SocketAddress(const SocketAddress& sock);
	~SocketAddress();

};

