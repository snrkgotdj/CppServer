#include "pch.h"
#include "ThreadPool.h"

#include "Task.h"

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
		return;

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
	std::unique_ptr<OverlappedCallback> pCallback = std::make_unique<OverlappedCallback>(callback);
	if (iocp_ != CreateIoCompletionPort(handle, iocp_, reinterpret_cast<ULONG_PTR>(pCallback.get()), 0))
	{
		PrintLastError();
		return false;
	}

	::InterlockedIncrement(&jobCount_);
	pCallback.release();
	return true;
}

void ThreadPool::threadFunc(ThreadPool* pPool, size_t index)
{
	if (pPool == nullptr)
		return;

	std::wstringstream ss;
	ss << pPool->name_ << L"_" << index << "\n";

	HRESULT result = SetThreadDescription(GetCurrentThread(), ss.view().data());
	OVERLAPPED_ENTRY entrys[20] = {};

	while (true)
	{
		ULONG count = 0;
		BOOL result = GetQueuedCompletionStatusEx(pPool->iocp_, entrys, 10, &count, INFINITE, FALSE);
		if (result == FALSE)
		{
			PrintLastError();
			continue;
		}

		bool isTerminate = false;
		for (ULONG i = 0; i < count; ++i)
		{
			OVERLAPPED_ENTRY& entry = entrys[i];
			FunctionType type = static_cast<FunctionType>(entry.dwNumberOfBytesTransferred);
			switch (type)
			{
			
			case FunctionType::FT_TERMINATE:
			{
				isTerminate = true;
				break;
			}
			
			case FunctionType::FT_CALLBACK:
			{
				std::unique_ptr<Task> pTask(reinterpret_cast<Task*>(entry.lpCompletionKey));
				if (pTask)
				{
					pTask->process();
				}
				
				::InterlockedDecrement(&pPool->jobCount_);
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
		if (isTerminate)
			return;
	}
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

			}
		}
	}

	for (size_t i = 0; i < threads_.size(); ++i)
	{
		threads_[i]->detach();
	}

	CloseHandle(iocp_);
	iocp_ = NULL;
}