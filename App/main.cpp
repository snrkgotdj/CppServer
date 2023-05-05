#include "stdafx.h"


int main()
{
    setlocale(LC_ALL, "");

    ThreadPool threadPool(L"testThread", 2);


    threadPool.queue([]() 
        {
            std::printf("test");
        });

    while (threadPool.getJobCount() != 0)
    {
        ::Sleep(1);
    }

}
