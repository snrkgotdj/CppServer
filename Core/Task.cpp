#include "pch.h"
#include "Task.h"

void CallbackTask::process()
{
	if (callback_ == nullptr)
		return;

	callback_();
}

void OverlappedTask::process()
{
	if (callback_ == nullptr)
		return;

	callback_(numberOfBytesTransferred, pOverlapped);
}
