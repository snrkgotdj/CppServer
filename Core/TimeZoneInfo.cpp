#include "pch.h"
#include "TimeZoneInfo.h"

TimeZoneInfo::CachedData TimeZoneInfo::g_cachedData;

TimeZoneInfo::CachedData::~CachedData()
{
    if (localTimeZone_)
    {
        delete localTimeZone_;
    }

    if (utcTimeZone_)
    {
        delete utcTimeZone_;
    }
}

const TimeZoneInfo& TimeZoneInfo::CachedData::createLocal()
{
    if (localTimeZone_ == nullptr)
    {
        // 로컬일 경우 한국으로 강제설정 다른나라의 셋팅이 필요하면 여기를 수정하면 됨
        localTimeZone_ = new TimeZoneInfo("KST Standard Time",
            "(GMT+9) Korean Time (SOUTH KOREA)",
            TimeSpan(9, 0, 0));
    }

    return *localTimeZone_;
}

const TimeZoneInfo& TimeZoneInfo::CachedData::getLocal()
{
    if (localTimeZone_ == nullptr)
    {
        return createLocal();
    }

    return *localTimeZone_;
}

TimeZoneInfo::TimeZoneInfo(std::string_view id, std::string_view displayName, TimeSpan baseUtcOffset)
    :id_(id)
    , displayName_(displayName)
    , baseUtcOffset_(baseUtcOffset)
{

}

TimeSpan TimeZoneInfo::GetUtcOffsetFromUtc(DateTime time, TimeZoneInfo zone)
{
    bool isDaylightSavings;
    return GetUtcOffsetFromUtc(time, zone, isDaylightSavings);
}

TimeSpan TimeZoneInfo::GetUtcOffsetFromUtc(DateTime time, TimeZoneInfo zone, bool& outIsDaylightSaving)
{
    bool isAmbiguousLocalDst;
    return GetUtcOffsetFromUtc(time, zone, outIsDaylightSaving, isAmbiguousLocalDst);
}

TimeSpan TimeZoneInfo::GetUtcOffsetFromUtc(DateTime time, TimeZoneInfo zone, bool& outIsDaylightSaving, bool& outIsAmbiguousLocalDst)
{
    outIsDaylightSaving = false;
    outIsAmbiguousLocalDst = false;

    TimeSpan baseOffset = zone.baseUtcOffset_;

    // TODO : 해외 타임존 작업이 필요하면 아래 주석 코드 추가 필요

//    int year;
//    AdjustmentRule rule;
//    
//    if (time > s_maxDateOnly) {
//        rule = zone.GetAdjustmentRuleForTime(DateTime.MaxValue);
//        year = 9999;
//    }
//    else if (time < s_minDateOnly) {
//        rule = zone.GetAdjustmentRuleForTime(DateTime.MinValue);
//        year = 1;
//    }
//    else {
//        DateTime targetTime = time + baseOffset;
//        
//        // As we get the associated rule using the adjusted targetTime, we should use the adjusted year (targetTime.Year) too as after adding the baseOffset, 
//        // sometimes the year value can change if the input datetime was very close to the beginning or the end of the year. Examples of such cases:
//        //      Libya Standard Time when used with the date 2011-12-31T23:59:59.9999999Z
//        //      "W. Australia Standard Time" used with date 2005-12-31T23:59:00.0000000Z
//        year = targetTime.Year;
//        
//        rule = zone.GetAdjustmentRuleForTime(targetTime);
//    }
//    
//    if (rule != null) {
//        baseOffset = baseOffset + rule.BaseUtcOffsetDelta;
//        if (rule.HasDaylightSaving) {
//            isDaylightSavings = GetIsDaylightSavingsFromUtc(time, year, zone.m_baseUtcOffset, rule, out isAmbiguousLocalDst, zone);
//            baseOffset += (isDaylightSavings ? rule.DaylightDelta : TimeSpan.Zero /* FUTURE: rule.StandardDelta */);
//        }
//    }

    return baseOffset;
}