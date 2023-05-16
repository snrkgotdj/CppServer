#pragma once

#include "RoomThread.h"
#include "RoomHandler.h"

class RoomManager
{
private:
	static std::atomic_uint64_t count_;
	static std::vector<RoomThread*> roomThreads_;
	static RoomHandler roomHandler_;

public:
	static void init(uint32_t roomCount);
	static bool isHandler(std::string_view uri) { return roomHandler_.isHandler(uri); }
	static bool dispatch(ProtocolContextPtr pContext);
	static bool queue(const std::function<void()>& callback);

private:
	RoomManager() = delete;

};

