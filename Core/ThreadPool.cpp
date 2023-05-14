#include "pch.h"
#include "ThreadPool.h"

#include "Task.h"

thread_local OVERLAPPED_ENTRY entrys[512] = {};

enum class FunctionType
{
	FT_TERMINATE = -2,
	FT_CALLBACK = -1,
};

ThreadPool::ThreadPool(std::wstring_view name, ThreadType threadType, size_t concurrent)
{
	init(name, threadType, concurrent);
}

ThreadPool::~ThreadPool()
{
	close();
}

bool ThreadPool::init(std::wstring_view name, ThreadType threadType, size_t concurrent)
{
	if (isValid())
		return false;

	if (512 < concurrent)
	{
		concurrent = std::thread::hardware_concurrency();
	}

	name_ = name;
	threadType_ = threadType;

	iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (iocp_ == NULL)
		return false;

	threads_.resize(concurrent);

	for (size_t n = 0; n < concurrent; ++n)
	{
		std::shared_ptr<std::thread> thread = std::make_shared<std::thread>(&ThreadPool::threadFunc, this, n);
		threads_[n] = ThreadPair(std::stop_source(), thread);
	}

	return true;
}

size_t ThreadPool::getJobCount()
{
	return ::InterlockedCompareExchange(&jobCount_, 0ULL, 0ULL);
}

bool ThreadPool::queue(const std::function<void()>& callback)
{
	std::unique_ptr<CallbackTask> pTask = std::make_unique<CallbackTask>();
	pTask->setCallback(callback);

	bool ret = PostQueuedCompletionStatus(iocp_, static_cast<DWORD>(FunctionType::FT_CALLBACK), reinterpret_cast<ULONG_PTR>(pTask.get()), nullptr);
	if (ret == false)
	{
		PrintLastError();
		return false;
	}

	::InterlockedIncrement(&jobCount_);
	pTask.release();
	return true;
}

bool ThreadPool::bind(HANDLE handle, const OverlappedCallback& callback)
{
	if (iocp_ != CreateIoCompletionPort(handle, iocp_, reinterpret_cast<ULONG_PTR>(callback), 0))
	{
		PrintLastError();
		return false;
	}

	::InterlockedIncrement(&jobCount_);
	return true;
}

void ThreadPool::threadFunc(ThreadPool* pPool, size_t index)
{
	if (pPool == nullptr)
		return;

	ThreadUtils::setThreadType(pPool->threadType_);

	std::stop_callback<std::function<void()>> callback(pPool->threads_[index].stopSource.get_token(),
		[]()
		{
			printf("thread close [%s]\n", __FUNCTION__);
		});

	std::wstringstream ss;
	ss << pPool->name_ << L"_" << index << "\n";

	HRESULT result = SetThreadDescription(GetCurrentThread(), ss.view().data());

	while (true)
	{
		if (ThreadPool::Execute_ReturnType::Terminate == pPool->execute(INFINITE))
			break;
	}

	pPool->threads_[index].stopSource.request_stop();

}

ThreadPool::Execute_ReturnType ThreadPool::execute(DWORD timeout /*= INFINITE*/)
{
	ULONG count = 0;
	BOOL result = GetQueuedCompletionStatusEx(iocp_, entrys, fetchCount_, &count, timeout, FALSE);
	if (result == FALSE)
	{
		PrintLastError();
		return ThreadPool::Execute_ReturnType::Empty;
	}

	Execute_ReturnType retType = 0 != count ? Execute_ReturnType::OK : Execute_ReturnType::Empty;

	for (ULONG i = 0; i < count; ++i)
	{
		OVERLAPPED_ENTRY& entry = entrys[i];
		FunctionType type = static_cast<FunctionType>(entry.dwNumberOfBytesTransferred);
		switch (type)
		{

		case FunctionType::FT_TERMINATE:
		{
			retType = ThreadPool::Execute_ReturnType::Terminate;
			break;
		}

		case FunctionType::FT_CALLBACK:
		{
			std::unique_ptr<Task> pTask(reinterpret_cast<Task*>(entry.lpCompletionKey));
			if (pTask)
			{
				pTask->process();
			}

			::InterlockedDecrement(&jobCount_);
			break;
		}

		default:
		{
			OverlappedCallback* pCallback = reinterpret_cast<OverlappedCallback*>(entry.lpCompletionKey);
			if (pCallback)
			{
				(*pCallback)(entry.dwNumberOfBytesTransferred, entry.lpOverlapped);
			}

			break;
		}
		}
	}
	
	return retType;
}

void ThreadPool::close()
{
	if (isValid() == false)
		return;

	for (size_t i = 0; i < threads_.size(); ++i)
	{
		::PostQueuedCompletionStatus(iocp_, static_cast<DWORD>(FunctionType::FT_TERMINATE), 0, nullptr);
		while (true)
		{
			size_t terminateCount = i + 1;
			for (size_t j = 0; j < threads_.size(); ++j)
			{
				ThreadPair threadPair = threads_[j];
				if (threadPair.stopSource.stop_requested())
				{
					terminateCount -= 1;
					if (threadPair.pThread->joinable())
					{
						threadPair.pThread->join();
					}
				}
			}

			if (terminateCount == 0)
				break;
			
			::Sleep(1);
		}
	}

	threads_.clear();

	CloseHandle(iocp_);
	iocp_ = NULL;
}