#include "pch.h"
#include "ThreadUtils.h"

__declspec(thread) int threadType_ = static_cast<int>(ThreadType::NONE);

namespace ThreadUtils
{
	bool isEqualThread(ThreadType threadType)
	{
		return threadType_ == static_cast<int>(threadType);
	}

	bool setThreadType(ThreadType threadType)
	{
		if (!isEqualThread(ThreadType::NONE))
		{
			return false;
		}

		threadType_ = static_cast<int>(threadType);
		return true;
	}

	uint64_t getCurrentThreadId()
	{
		return ::GetCurrentThreadId();
	}
}