#pragma once

enum class ThreadType
{
	NONE = 0,
	MAIN,
	DB,
	LOG,
	NETWORK,
	LOGIC,
};

namespace ThreadUtils
{
	bool isEqualThread(ThreadType threadType);

	bool setThreadType(ThreadType threadType);

	uint64_t getCurrentThreadId();
}