#pragma once

#include "TimeSpan.h"
#include "TimeInternal.h"

enum class DateTimeKind
{
    UNSPECIFIED = 0,
    UTC = 1,
    LOCAL = 2,
};

enum class DayOfWeek
{
    Monday = 1,
    Tuesday = 2,
    Wednesday = 3,
    Thursday = 4,
    Friday = 5,
    Saturday = 6,
    Sunday = 7,
};

class DateTime
{
public:
    typedef std::chrono::duration<long long, std::nano> nanoseconds;
    typedef std::chrono::duration<long long, std::micro> microseconds;
    typedef std::chrono::duration<long long, std::milli> milliseconds;
    typedef std::chrono::duration<long long> seconds;
    typedef std::chrono::duration<long long, std::ratio<60>> minutes;
    typedef std::chrono::duration<long long, std::ratio<3600> > hours;
    typedef std::chrono::duration<long long, std::ratio<3600 * 24> > days;
    typedef std::chrono::duration<long long, std::ratio<3600 * 24 * 7> > weeks;

private:
    static std::function<DateTime()> nowFunc_;

private:
    unsigned long long dateData_ = 0;

public:
    static void init(std::function<DateTime()> nowFunc) { nowFunc_ = nowFunc; }

public:
    static DateTime createWithTicks(long long ticks, DateTimeKind kind);
    static DateTime epochTime(); // 1970.01.01 00:00:00 시간
    static DateTime utcNow();
    static DateTime kstNow();
    static DateTime now(); // init 호출 후 사용할것

public:
    bool isLeapYear();

public:
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    int getDayOfYear() const;
    int getHour() const;
    int getHour12() const;
    int getMinute() const;
    int getSecond() const;
    int getMillisecond() const;
    long long ticks() const;
    DateTimeKind kind() const;
    DayOfWeek dayOfWeek() const;

    long long getMillisSinceEpoch();

public:
    DateTime addYears(int year) const;
    DateTime addMonths(int month) const;
    DateTime addDays(double days) const;
    DateTime addHours(double hours) const;
    DateTime addMinutes(double minutes) const;
    DateTime addSeconds(double seconds) const;
    DateTime addMilliSeconds(double millis) const;

public:
    std::string toString() const;
    std::string toString(const char* Format) const;

public:
    bool operator == (const DateTime& rhs) const;
    bool operator != (const DateTime& rhs) const;
    bool operator < (const DateTime& rhs) const;
    bool operator <= (const DateTime& rhs) const;
    bool operator > (const DateTime& rhs) const;
    bool operator >= (const DateTime& rhs) const;
    TimeSpan operator - (const DateTime& rhs) const;

public:
    template <typename T>
    DateTime ceil() const;

    template <typename T>
    DateTime floor() const;

public:
    DateTime() = default;
    DateTime(long long millis, DateTimeKind kind = DateTimeKind::LOCAL);
    DateTime(int year, int month, int day, DateTimeKind kind);
    DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind);
    DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind);
};

template <typename T>
DateTime DateTime::ceil() const
{
    long long micro = this->ticks() / TimeInternal::g_ticksPerMicrosecond;

    std::chrono::duration<long long, std::micro> duration(micro);
    std::chrono::duration<long long, std::micro> ceilDuration = std::chrono::ceil<T>(duration);

    DateTime date = DateTime::createWithTicks(ceilDuration.count() * TimeInternal::g_ticksPerMicrosecond, this->kind());
    return date;
}

template <typename T>
DateTime DateTime::floor() const
{
    long long micro = this->ticks() / TimeInternal::g_ticksPerMicrosecond;

    std::chrono::duration<long long, std::micro> duration(micro);
    std::chrono::duration<long long, std::micro> ceilDuration = std::chrono::floor<T>(duration);

    DateTime date = DateTime::createWithTicks(ceilDuration.count() * TimeInternal::g_ticksPerMicrosecond, this->kind());
    return date;
}