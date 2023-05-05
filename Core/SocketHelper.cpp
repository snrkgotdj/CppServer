#include "pch.h"
#include "SocketHelper.h"

bool SocketHelper::initSockets()
{
	WSAData wsaData = {};
	if (0 == WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		PrintLastError();
		return false;
	}

	return true;
}

void SocketHelper::closeSockets()
{
	WSACleanup();
}
