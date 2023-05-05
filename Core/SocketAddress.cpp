#include "pch.h"
#include "SocketAddress.h"

SocketAddress::SocketAddress()
	: buffer_(nullptr)
	, bufferMaxSize_(0)
	, bufferLength_(0)
{
	
}

SocketAddress::SocketAddress(int64_t ip, AddressFamily addressFamily, int32_t port)
	: buffer_(nullptr)
	, bufferMaxSize_(0)
	, bufferLength_(0)
{
	init(ip, addressFamily, port);
}

SocketAddress::SocketAddress(const SocketAddress& sock)
	: buffer_(nullptr)
	, bufferMaxSize_(sock.bufferMaxSize_)
	, bufferLength_(sock.bufferLength_)
{
	if (sock.bufferLength_ == 0)
		return;

	initBuffer(AddressFamily::INET);
	memcpy_s(buffer_, bufferMaxSize_, sock.buffer_, sock.bufferLength_);
}

SocketAddress::~SocketAddress()
{
	destroy();
}

void SocketAddress::initBuffer(AddressFamily addressFamily)
{
	if (buffer_ == nullptr)
	{
		bufferMaxSize_ = sizeof(SOCKADDR_IN6) > sizeof(SOCKADDR_IN) ? sizeof(SOCKADDR_IN6) : sizeof(SOCKADDR_IN);
		buffer_ = new char[bufferMaxSize_];
	}

	bufferLength_ = 0;
	memset(buffer_, 0x00, bufferMaxSize_);
}

void SocketAddress::init(int64_t ip, AddressFamily addressFamily, int32_t port)
{
	initBuffer(addressFamily);

	bufferLength_ = sizeof(SOCKADDR_IN6);
	if(addressFamily == AddressFamily::INET)
	{
		bufferLength_ = sizeof(SOCKADDR_IN);
	}

	int family = static_cast<int32_t>(addressFamily);

	buffer_[0] = static_cast<char>(family);
	buffer_[1] = static_cast<char>(family >> 8);

	buffer_[2] = (char)(port >> 8);
	buffer_[3] = (char)(port);

	buffer_[4] = (char)(ip);
	buffer_[5] = (char)(ip >> 8);
	buffer_[6] = (char)(ip >> 16);
	buffer_[7] = (char)(ip >> 24);
}

void SocketAddress::destroy()
{
	if (buffer_)
	{
		delete buffer_;
		buffer_ = nullptr;
	}

	bufferMaxSize_ = 0;
	bufferLength_ = 0;
}

