
#include "stdafx.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    setlocale(LC_ALL, "");

    SocketHelper::initSockets();

    DNSEndPoint endPoint("www.naver.com", 80);

    SocketAddress sockAddr = endPoint.serialize();

    printf("%s\n", endPoint.getIPString().c_str());

    SocketHelper::closeSockets();
}
