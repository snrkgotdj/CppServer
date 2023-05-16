#pragma once

#include "Dispatcher.h"

class RoomHandler
{
private:
	Dispatcher dispatcher_;

public:
	bool init();
	bool isHandler(std::string_view uri);
	bool dispatch(ProtocolContextPtr pContext);

private:
	template<class T>
	void onTemplateExecute(ProtocolContextPtr pContext)
	{
		std::shared_ptr<T> pShared = std::make_shared<T>(pContext);
		pShared->Start();
	}

public:
	RoomHandler() = default;
};