#include "pch.h"
#include "TimeSpan.h"

TimeSpan TimeSpan::Zero = TimeSpan(0);

TimeSpan TimeSpan::MaxValue = TimeSpan(LLONG_MAX);
TimeSpan TimeSpan::MinValue = TimeSpan(LLONG_MIN);

long long TimeSpan::getTotalMilliseconds()
{
    double temp = (double)ticks_ * MillisecondsPerTick;
    if (temp > MaxMilliSeconds)
        return (double)MaxMilliSeconds;

    if (temp < MinMilliSeconds)
        return (double)MinMilliSeconds;

    return static_cast<long long>(temp);
}

long long TimeSpan::getTotalSeconds()
{
    return static_cast<long long>(ticks_ * SecondsPerTick);
}

std::string TimeSpan::toString() const
{
    if (getDays() == 0)
    {
        return toString("%H:%M:%S");
    }

    return toString("%d일 %H:%M:%S");
}

#include <iomanip>

std::string TimeSpan::toString(const char* format) const
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
            case 'd': result << getDays(); break;
            case 'H': result << getHours(); break;
            case 'M': result << getMinutes(); break;
            case 'S': result << getSeconds(); break;
            case 'f': result << getMillis(); break;
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

long long TimeSpan::timeToTicks(int hour, int minute, int second)
{
    long totalSeconds = (long)hour * 3600 + (long)minute * 60 + (long)second;
    return totalSeconds * TicksPerSecond;
}

TimeSpan TimeSpan::operator - (const TimeSpan& rhs) const
{
    return TimeSpan(getTicks() - rhs.getTicks());
}