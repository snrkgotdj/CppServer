#include "stdafx.h"
#include "RoomManager.h"

std::atomic_uint64_t RoomManager::count_ = 0;
std::vector<RoomThread*> RoomManager::roomThreads_;
RoomHandler RoomManager::roomHandler_;

void RoomManager::init(uint32_t roomCount)
{
	for (uint32_t n = 0; n < roomCount; ++n)
	{
		roomThreads_.push_back(new RoomThread());
	}
	
	roomHandler_.init();
}

bool RoomManager::dispatch(ProtocolContextPtr pContext)
{
	bool ret = queue([pContext]() { roomHandler_.dispatch(pContext); });
	return ret;
}

bool RoomManager::queue(const std::function<void()>& callback)
{
	uint64_t roomIndex = ++count_ % roomThreads_.size();
	bool ret = roomThreads_[roomIndex]->queue(callback);

	return ret;
}
