#include "pch.h"
#include "DateTime.h"

#include "TimeZoneInfo.h"

std::function<DateTime()> DateTime::nowFunc_ = nullptr;

DateTime::DateTime(long long millis, DateTimeKind kind)
    :DateTime(1970, 1, 1, 0, 0, 0, kind)
{
    dateData_ = TimeInternal::add(dateData_, millis, 1);

    if (DateTimeKind::LOCAL == kind)
    {
        long long ticks = TimeInternal::internalTicks(dateData_);
        long long offsetTicks = TimeZoneInfo::GetUtcOffsetFromUtc(*this, TimeZoneInfo::getLocal()).getTicks();

        ticks += offsetTicks;

        *this = DateTime::createWithTicks(ticks, kind);
    }
}

DateTime::DateTime(int year, int month, int day, DateTimeKind kind)
{
    if (false == TimeInternal::isValidate(year, month, day))
    {
        assert(0); // DateTime Constructor Failed 파라미터로 넘어온 year, month, day 가 이상합니다.
    }

    dateData_ = static_cast<unsigned long long>(TimeInternal::dateToTicks(year, month, day));

    switch (kind)
    {
    case DateTimeKind::UTC:
        dateData_ |= TimeInternal::g_kindUtc;
        break;

    case DateTimeKind::LOCAL:
        dateData_ |= TimeInternal::g_kindLocal;
        break;

    case DateTimeKind::UNSPECIFIED:
        dateData_ |= TimeInternal::g_kindUnspecified;
        break;

    default:
        assert(0); // DateTime Constructor Failed 지원하지 않는 DateTimeKind 입니다.
        break;
    }
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, DateTimeKind kind)
{
    if (false == TimeInternal::isValidate(year, month, day, hour, minute, second, 0))
    {
        assert(0); // DateTime Constructor Failed 파라미터로 넘어온 year, month, day, hour, minute, second 가 이상합니다.
    }

    dateData_ = static_cast<unsigned long long>(TimeInternal::dateToTicks(year, month, day) + TimeInternal::timeToTicks(hour, minute, second, 0));

    switch (kind)
    {
    case DateTimeKind::UTC:
        dateData_ |= TimeInternal::g_kindUtc;
        break;

    case DateTimeKind::LOCAL:
        dateData_ |= TimeInternal::g_kindLocal;
        break;

    case DateTimeKind::UNSPECIFIED:
        dateData_ |= TimeInternal::g_kindUnspecified;
        break;

    default:
        assert(0); // DateTime Constructor Failed 지원하지 않는 DateTimeKind 입니다.
        break;
    }
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second, int millisecond, DateTimeKind kind)
{
    if (false == TimeInternal::isValidate(year, month, day, hour, minute, second, millisecond))
    {
        assert(0); // DateTime Constructor Failed 파라미터로 넘어온 year, month, day, hour, minute, second, millisecond 가 이상합니다.
    }

    dateData_ = static_cast<unsigned long long>(TimeInternal::dateToTicks(year, month, day) + TimeInternal::timeToTicks(hour, minute, second, millisecond));

    switch (kind)
    {
    case DateTimeKind::UTC:
        dateData_ |= TimeInternal::g_kindUtc;
        break;

    case DateTimeKind::LOCAL:
        dateData_ |= TimeInternal::g_kindLocal;
        break;

    case DateTimeKind::UNSPECIFIED:
        dateData_ |= TimeInternal::g_kindUnspecified;
        break;

    default:
        assert(0); // DateTime Constructor Failed 지원하지 않는 DateTimeKind 입니다.
        break;
    }
}

DateTime DateTime::createWithTicks(long long ticks, DateTimeKind kind)
{
    if (ticks <TimeInternal::g_minTicks || ticks > TimeInternal::g_maxTicks)
    {
        assert(0); // DateTime::createWithTicks Failed 파라미터로 넘어온 ticks 값이 이상합니다.
    }

    DateTime date;
    date.dateData_ = static_cast<unsigned long long>(ticks);

    switch (kind)
    {
    case DateTimeKind::UTC:
        date.dateData_ |= TimeInternal::g_kindUtc;
        break;

    case DateTimeKind::LOCAL:
        date.dateData_ |= TimeInternal::g_kindLocal;
        break;

    case DateTimeKind::UNSPECIFIED:
        date.dateData_ |= TimeInternal::g_kindUnspecified;
        break;
    default:
        assert(0); // DateTime Constructor Failed 지원하지 않는 DateTimeKind 입니다.
        break;
    }

    return date;
}

DateTime DateTime::epochTime()
{
    return DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::UTC);
}

DateTime DateTime::utcNow()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration duration = now.time_since_epoch();
    std::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    return DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::UTC).addMilliSeconds(millis.count());
}

DateTime DateTime::kstNow()
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::chrono::system_clock::duration duration = now.time_since_epoch();
    std::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

    return DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind::LOCAL).addMilliSeconds(millis.count()).addHours(9);
}

DateTime DateTime::now()
{
    if (nullptr == nowFunc_)
    {
        assert(0); // DateTime::init을 호출하고 쓸것
        return kstNow();
    }

    return nowFunc_();
}

bool DateTime::isLeapYear()
{
    int year = getYear();
    bool ret = TimeInternal::isLeapYear(year);
    return ret;
}

int DateTime::getYear() const
{
    return TimeInternal::getDatePart(dateData_, TimeInternal::g_datePartYear);
}

int DateTime::getMonth() const
{
    return TimeInternal::getDatePart(dateData_, TimeInternal::g_datePartMonth);
}

int DateTime::getDay() const
{
    return TimeInternal::getDatePart(dateData_, TimeInternal::g_datePartDay);
}

int DateTime::getDayOfYear() const
{
    int year = getYear();
    int month = getMonth();
    int day = getDay();

    for (int CurrentMonth = 1; CurrentMonth < month; ++CurrentMonth)
    {
        day += TimeInternal::daysInMonth(year, CurrentMonth);
    }

    return day;
}

int DateTime::getHour() const
{
    return static_cast<int>((TimeInternal::internalTicks(dateData_) / TimeInternal::g_ticksPerHour) % 24);
}

int DateTime::getHour12() const
{
    int hour = getHour();

    if (hour < 1)
    {
        return 12;
    }

    if (hour > 12)
    {
        return (hour - 12);
    }

    return hour;
}

int DateTime::getMinute() const
{
    return static_cast<int>((TimeInternal::internalTicks(dateData_) / TimeInternal::g_ticksPerMinute) % 60);
}

int DateTime::getSecond() const
{
    return static_cast<int>((TimeInternal::internalTicks(dateData_) / TimeInternal::g_ticksPerSecond) % 60);
}

int DateTime::getMillisecond() const
{
    return static_cast<int>((TimeInternal::internalTicks(dateData_) / TimeInternal::g_ticksPerMillisecond) % 1000);
}

long long DateTime::ticks() const
{
    return TimeInternal::internalTicks(dateData_);
}

DateTimeKind DateTime::kind() const
{
    switch (TimeInternal::internalKind(dateData_))
    {
    case TimeInternal::g_kindLocal:
        return DateTimeKind::LOCAL;
    case TimeInternal::g_kindUtc:
        return DateTimeKind::UTC;
    case TimeInternal::g_kindUnspecified:
    default:
        return DateTimeKind::UNSPECIFIED;
    }
}

DayOfWeek DateTime::dayOfWeek() const
{
    return static_cast<::DayOfWeek>(((TimeInternal::internalTicks(dateData_) / TimeInternal::g_ticksPerDay + 1) % 7) + 1);
}

long long DateTime::getMillisSinceEpoch()
{
    TimeSpan timeSpan = (*this) - DateTime::epochTime();

    if (DateTimeKind::LOCAL == kind())
    {
        TimeSpan offset = TimeZoneInfo::GetUtcOffsetFromUtc(*this, TimeZoneInfo::getLocal());
        timeSpan = timeSpan - offset;
    }

    long long millis = timeSpan.getTotalMilliseconds();
    return millis;
}

DateTime DateTime::addYears(int year) const
{
    if (year < -10000 || year > 10000)
    {
        assert(0);
    }

    return addMonths(year * 12);
}

DateTime DateTime::addMonths(int months) const
{
    if (months < -120000 || months > 120000)
    {
        assert(0);
    }

    int y, m, d;
    TimeInternal::getDatePart(dateData_, y, m, d);

    int i = m - 1 + months;
    if (i >= 0)
    {
        m = i % 12 + 1;
        y = y + i / 12;
    }
    else
    {
        m = 12 + (i + 1) % 12;
        y = y + (i - 11) / 12;
    }
    if (y < 1 || y > 9999)
    {
        assert(0);
    }
    int days = TimeInternal::daysInMonth(y, m);
    if (d > days)
    {
        d = days;
    }

    DateTime ret;
    ret.dateData_ = (TimeInternal::dateToTicks(y, m, d) + TimeInternal::internalTicks(dateData_) % TimeInternal::g_ticksPerDay) | TimeInternal::internalKind(dateData_);
    return ret;
}

DateTime DateTime::addDays(double days) const
{
    DateTime ret;
    ret.dateData_ = TimeInternal::add(dateData_, days, TimeInternal::g_millisPerDay);
    return ret;
}

DateTime DateTime::addHours(double hours) const
{
    DateTime ret;
    ret.dateData_ = TimeInternal::add(dateData_, hours, TimeInternal::g_millisPerHour);
    return ret;
}

DateTime DateTime::addMinutes(double minutes) const
{
    DateTime ret;
    ret.dateData_ = TimeInternal::add(dateData_, minutes, TimeInternal::g_millisPerMinute);
    return ret;
}

DateTime DateTime::addSeconds(double seconds) const
{
    DateTime ret;
    ret.dateData_ = TimeInternal::add(dateData_, seconds, TimeInternal::g_millisPerSecond);
    return ret;
}

DateTime DateTime::addMilliSeconds(double millis) const
{
    DateTime ret;
    ret.dateData_ = TimeInternal::add(dateData_, millis, 1);
    return ret;
}

bool DateTime::operator == (const DateTime& rhs) const
{
    return dateData_ == rhs.dateData_;
}

bool DateTime::operator != (const DateTime& rhs) const
{
    return dateData_ != rhs.dateData_;
}

bool DateTime::operator < (const DateTime& rhs) const
{
    return dateData_ < rhs.dateData_;
}

bool DateTime::operator <= (const DateTime& rhs) const
{
    return dateData_ <= rhs.dateData_;
}

bool DateTime::operator > (const DateTime& rhs) const
{
    return dateData_ > rhs.dateData_;
}

bool DateTime::operator >= (const DateTime& rhs) const
{
    return dateData_ >= rhs.dateData_;
}

TimeSpan DateTime::operator - (const DateTime& rhs) const
{
    return TimeSpan(ticks() - rhs.ticks());
}

std::string DateTime::toString() const
{
    return toString("%Y-%m-%d %H:%M:%S");
}

#include <iomanip>

std::string DateTime::toString(const char* format) const
{
    std::stringstream result;

    while (*format != '\0')
    {
        if ((*format == '%') && (*++format != '\0'))
        {
            // %-가 아니면 0으로 패딩후 두자리 보장
            if (*format != '-')
            {
                if (*format != 'Y' && *format != 'f')
                {
                    result << std::setfill('0') << std::setw(2);
                }
            }
            else
            {
                format += 1;
            }

            switch (*format)
            {
            case 'Y': result << getYear(); break;
            case 'y': result << getYear() % 100; break;
            case 'm': result << getMonth(); break;
            case 'd': result << getDay(); break;
            case 'H': result << getHour(); break;
            case 'M': result << getMinute(); break;
            case 'S': result << getSecond(); break;
            case 'f': result << getMillisecond(); break;
            default:
                result << *format;
            }
        }
        else
        {
            result << *format;
        }

        ++format;
    }

    return result.str();
}