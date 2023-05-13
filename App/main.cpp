
#include "stdafx.h"

#include "IPAddressUtils.h"
#include "DateTime.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock")

int main()
{
    setlocale(LC_ALL, "");

    

    SocketHelper::initSockets();

    TCPListener listener;
    listener.init(IPEndPoint(IPAddressUtils::parse("0.0.0.0"), 30000));
    
    listener.start();

    std::string str = DateTime::kstNow().toString();


    while (true)
    {
        ::Sleep(1);
    }


    SocketHelper::closeSockets();
}
