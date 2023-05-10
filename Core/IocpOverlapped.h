#pragma once

class IocpOverlapped
{
private:
	WSAOVERLAPPED overlapped_;

public:
	virtual void excute(DWORD numberOfBytes) = 0;
	LPWSAOVERLAPPED getOverlapped() { return &overlapped_; }

public:
	IocpOverlapped();
	virtual ~IocpOverlapped() = 0 {}
};

class AcceptOverlapped : public IocpOverlapped
{
private:
	NetSocketPtr pSocket_;
	char buffer_[128];

public:
	void excute(DWORD numberOfBytes) override;

public:
	PVOID getBuffer() { return buffer_; }
	NetSocketPtr getSocket() { return pSocket_; }

public:
	AcceptOverlapped(NetSocketPtr pSocket)
		: pSocket_(pSocket)
	{
		memset(buffer_, 0, sizeof(buffer_));
	}

	~AcceptOverlapped() = default;
};

class ReceiveOverlapped : public IocpOverlapped
{
private:
	NetSocket* pSocket_;
	WSABUF wsaBuffer_;

public:
	void excute(DWORD numberOfBytes) override;

public:
	void clear();

public:
	LPWSABUF getWsaBuf() { return &wsaBuffer_; };

public:
	ReceiveOverlapped() = default;

	ReceiveOverlapped(NetSocket* pSocket)
		: pSocket_(pSocket)
	{
		wsaBuffer_.buf = nullptr;
		wsaBuffer_.len = 0;
		clear();
	}

	~ReceiveOverlapped()
	{
		delete wsaBuffer_.buf;
		wsaBuffer_.buf = nullptr;
	}
};

class DisconnectOverlapped : public IocpOverlapped
{
private:
	NetSocket* pSocket_;

public:
	void excute(DWORD numberOfBytes) override;

public:
	DisconnectOverlapped() = default;
	DisconnectOverlapped(NetSocket* pSocket)
		:pSocket_(pSocket)
	{

	}

	~DisconnectOverlapped() {}
};