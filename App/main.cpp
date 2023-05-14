
#include "stdafx.h"

#include "IPAddressUtils.h"
#include "DateTime.h"
#include "LogManager.h"

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Mswsock")

int main()
{
    setlocale(LC_ALL, "");

    LogManager::getInstance()->init("C:\\Users\\djHome\\Documents\\work\\CppServer\\x64\\", "Log", LOG_TYPE::LT_DEBUG);
    MY_LOG(LOG_TYPE::LT_INFO, "%s", "ServerStart");

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
