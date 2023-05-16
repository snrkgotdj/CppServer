#pragma once

#include "Dispatcher.h"

class ServerHandler
{
private:
	Dispatcher dispatcher_;

public:
	bool init();
	bool dispatch(ProtocolContextPtr pContext);

private:
	template<class T>
	void onTemplateExecute(ProtocolContextPtr pContext)
	{
		std::shared_ptr<T> pShared = std::make_shared<T>(pContext);
		pShared->start();
	}
};

