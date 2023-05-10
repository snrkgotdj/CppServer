#pragma once

class IPAddressUtils final
{
public:
	static int64_t parse(std::string_view ipstring);


private:
	IPAddressUtils() = delete;
};