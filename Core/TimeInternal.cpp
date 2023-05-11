#include "pch.h"
#include "TimeInternal.h"

long long TimeInternal::dateToTicks(int year, int month, int day)
{
    if (year >= 1 && year <= 9999 && month >= 1 && month <= 12)
    {
        const int* days = isLeapYear(year) ? g_daysToMonth366 : g_daysToMonth365;
        if (day >= 1 && day <= days[month] - days[month - 1])
        {
            int y = year - 1;
            int n = y * 365 + y / 4 - y / 100 + y / 400 + days[month - 1] + day - 1;
            return n * g_ticksPerDay;
        }
    }

    return 0;
}

long long TimeInternal::timeToTicks(int hour, int minute, int second, int millisecond)
{
    if (hour >= 0 && hour < 24 && minute >= 0 && minute < 60 && second >= 0 && second < 60 && millisecond >= 0 && millisecond < 1000)
    {
        return hour * g_ticksPerHour
            + minute * g_ticksPerMinute
            + second * g_ticksPerSecond
            + millisecond * g_ticksPerMillisecond;
    }

    return -1;
}

long long TimeInternal::millisecondToTicks(long long millisecond)
{
    return g_ticksPerMillisecond * millisecond;
}

bool TimeInternal::isLeapYear(int year)
{
    if (year < 1 || year > 9999)
    {
        year = 1;
    }

    return year % 4 == 0 && (year % 100 != 0 || year % 400 == 0);
}

long long TimeInternal::assign(int days, int hours, int minutes, int seconds, int millis, int micros, int fractionNano)
{
    long long totalTicks = 0;

    totalTicks += days * g_ticksPerDay;
    totalTicks += hours * g_ticksPerHour;
    totalTicks += minutes * g_ticksPerMinute;
    totalTicks += seconds * g_ticksPerSecond;
    totalTicks += millis * g_ticksPerMillisecond;
    totalTicks += micros * g_ticksPerMicrosecond;
    totalTicks += fractionNano / g_nanosecondsPerTick;

    return totalTicks;
}

long long TimeInternal::internalTicks(unsigned long long dateData)
{
    return static_cast<long long>(dateData & g_ticksMask);
}

unsigned long long TimeInternal::internalKind(unsigned long long dateData)
{
    return dateData & g_flagsMask;
}

int TimeInternal::getDatePart(unsigned long long dateData, int part)
{
    unsigned long long epocheTime = static_cast<unsigned long long>(TimeInternal::dateToTicks(1970, 1, 1));
    long long ticks = TimeInternal::internalTicks(dateData);

    // tm을 사용하기 위해 초단위로 변경
    time_t seconds = static_cast<time_t>((ticks - epocheTime) / g_ticksPerSecond);
    tm tstruct;

    gmtime_s(&tstruct , &seconds);
    tstruct.tm_year += 1900;
    tstruct.tm_mon += 1;

    if (part == g_datePartYear)
        return tstruct.tm_year;

    if (part == g_datePartDayOfYear)
        return tstruct.tm_yday;

    if (part == g_datePartMonth)
        return tstruct.tm_mon;

    return tstruct.tm_mday;
}

void TimeInternal::getDatePart(unsigned long long dateData, int& year, int& month, int& day)
{
    unsigned long long epocheTime = static_cast<unsigned long long>(TimeInternal::dateToTicks(1970, 1, 1));
    long long ticks = TimeInternal::internalTicks(dateData);

    // tm을 사용하기 위해 초단위로 변경
    time_t seconds = static_cast<time_t>((ticks - epocheTime) / g_ticksPerSecond);
    tm tstruct;

    gmtime_s(&tstruct, &seconds);
    tstruct.tm_year += 1900;
    tstruct.tm_mon += 1;

    year = tstruct.tm_year;
    month = tstruct.tm_mon;
    day = tstruct.tm_mday;

    return;
}

int TimeInternal::daysInYear(int year)
{
    return isLeapYear(year) ? 366 : 365;
}

int TimeInternal::daysInMonth(int year, int month)
{
    if (month < 1 || month > 12)
    {
        assert(false); // 잘못된 Month값이 들어왔습니다.
        return 0;
    }

    const int* days = isLeapYear(year) ? g_daysToMonth366 : g_daysToMonth365;
    int day = days[month] - days[month - 1];
    return day;
}

bool TimeInternal::isValidate(int year, int month, int day)
{
    return (year >= 1) && (year <= 9999) &&
        (month >= 1) && (month <= 12) &&
        (day >= 1) && (day <= daysInMonth(year, month));
}

bool TimeInternal::isValidate(int year, int month, int day, int hour, int minute, int second, int millis)
{
    return (year >= 1) && (year <= 9999) &&
        (month >= 1) && (month <= 12) &&
        (day >= 1) && (day <= daysInMonth(year, month)) &&
        (hour >= 0) && (hour <= 23) &&
        (minute >= 0) && (minute <= 59) &&
        (second >= 0) && (second <= 59) &&
        (millis >= 0) && (millis <= 999);
}

unsigned long long TimeInternal::add(unsigned long long dateData, double value, int scale)
{
    long long millis = static_cast<long long>(value * scale + (value >= 0 ? 0.5 : -0.5));
    if (millis <= -g_maxMillis || millis >= g_maxMillis)
    {
        assert(false); // TimeInternal::Add Failed 파라미터로 넘어온 value 값이 이상합니다.
    }

    return addTicks(dateData, millis * g_ticksPerMillisecond);
}

unsigned long long TimeInternal::addTicks(unsigned long long dateData, long long value)
{
    long long ticks = internalTicks(dateData);
    if (value > g_maxTicks - ticks || value < g_minTicks - ticks)
    {
        assert(false); // TimeInternal::AddTicks Failed 파라미터로 넘어온 value 값이 이상합니다.
    }

    unsigned long long i64InternalKind = internalKind(dateData);

    return ticks + value | i64InternalKind;
}