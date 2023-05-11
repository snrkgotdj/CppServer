#pragma once

#include "DateTime.h"

class TimeZoneInfo
{
    class CachedData
    {
    private:
        TimeZoneInfo* localTimeZone_ = nullptr;
        TimeZoneInfo* utcTimeZone_ = nullptr;

    private:
        const TimeZoneInfo& createLocal();

    public:
        const TimeZoneInfo& getLocal();

    public:
        ~CachedData();
    };

private:
    static CachedData g_cachedData;

private:
    std::string id_;
    std::string displayName_;
    TimeSpan baseUtcOffset_;

public:
    static const TimeZoneInfo& getLocal() { return g_cachedData.getLocal(); }

public:
    static TimeSpan GetUtcOffsetFromUtc(DateTime time, TimeZoneInfo zone);
    static TimeSpan GetUtcOffsetFromUtc(DateTime time, TimeZoneInfo zone, bool& outIsDaylightSaving);
    static TimeSpan GetUtcOffsetFromUtc(DateTime time, TimeZoneInfo zone, bool& outIsDaylightSaving, bool& outIsAmbiguousLocalDst);


public:
    TimeZoneInfo(std::string_view id, std::string_view displayName, TimeSpan baseUtcOffset);
};