#include "stdafx.h"
#include "Dispatcher.h"

bool Dispatcher::dispatch(std::string_view uri, ProtocolContextPtr pContext)
{
	auto iter = handlers_.find(uri.data());
	if (iter == handlers_.end())
		return false;

	iter->second(pContext);
	return true;
}

bool Dispatcher::addHandler(std::string_view uri, const MessageHandler& handler)
{
    auto resultPair = handlers_.insert(std::make_pair(uri.data(), handler));
    return resultPair.second;
}

bool Dispatcher::getHandler(std::string_view uri, MessageHandler& handler)
{
	auto iter = handlers_.find(uri.data());
	if (iter == handlers_.end())
		return false;

	handler = iter->second;
	return true;
}