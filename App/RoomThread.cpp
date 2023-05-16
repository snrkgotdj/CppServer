#include "stdafx.h"
#include "RoomThread.h"

bool RoomThread::queue(const std::function<void()>& callback)
{
    bool ret = threadPool_.queue(callback);
    return ret;
}

void RoomThread::execute()
{
	while (true)
	{
		ThreadPool::Execute_ReturnType ret = threadPool_.execute(1);
		if (ret == ThreadPool::Execute_ReturnType::Terminate)
		{
			break;
		}
		if (ret == ThreadPool::Execute_ReturnType::OK)
		{
			continue;
		}

		// delta time

		//....
	}
}

void RoomThread::run(RoomThread* _this)
{
	if (_this == nullptr)
		return;

	_this->execute();
}
