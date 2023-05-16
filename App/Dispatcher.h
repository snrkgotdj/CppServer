#pragma once

using MessageHandler = std::function<void(ProtocolContextPtr)>;

class Dispatcher final
{
private:
	std::unordered_map<std::string, MessageHandler> handlers_;

public:
	bool dispatch(std::string_view uri, ProtocolContextPtr pContext);

public:
	bool addHandler(std::string_view uri, const MessageHandler& handler);
	bool getHandler(std::string_view uri, MessageHandler& handler);

};

