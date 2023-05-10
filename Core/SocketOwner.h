#pragma once

class SocketOwner
{
public:
	virtual bool isListener() { return false; }
	virtual void disconnect(NetSocketPtr pSocket) = 0;

public:
	virtual ~SocketOwner() = 0 {};
};

