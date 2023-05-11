#pragma once

class TimeSpan
{
public:
    static constexpr long long TicksPerMillisecond = 10000;
    static constexpr long long TicksPerSecond = TicksPerMillisecond * 1000;   // 10,000,000
    static constexpr long long TicksPerMinute = TicksPerSecond * 60;         // 600,000,000
    static constexpr long long TicksPerHour = TicksPerMinute * 60;        // 36,000,000,000
    static constexpr long long TicksPerDay = TicksPerHour * 24;          // 864,000,000,000

    static TimeSpan Zero;
    static TimeSpan MaxValue;
    static TimeSpan MinValue;

private:
    static constexpr double MillisecondsPerTick = 1.0 / TicksPerMillisecond;
    static constexpr double SecondsPerTick = 1.0 / TicksPerSecond;         // 0.0001
    static constexpr double MinutesPerTick = 1.0 / TicksPerMinute; // 1.6666666666667e-9
    static constexpr double HoursPerTick = 1.0 / TicksPerHour; // 2.77777777777777778e-11
    static constexpr double DaysPerTick = 1.0 / TicksPerDay; // 1.1574074074074074074e-12

    static constexpr int MillisPerSecond = 1000;
    static constexpr int MillisPerMinute = MillisPerSecond * 60; //     60,000
    static constexpr int MillisPerHour = MillisPerMinute * 60;   //  3,600,000
    static constexpr int MillisPerDay = MillisPerHour * 24;      // 86,400,000

    static constexpr long long MaxSeconds = LLONG_MAX / TicksPerSecond;
    static constexpr long long MinSeconds = LLONG_MIN / TicksPerSecond;

    static constexpr long long MaxMilliSeconds = LLONG_MAX / TicksPerMillisecond;
    static constexpr long long MinMilliSeconds = LLONG_MIN / TicksPerMillisecond;

    static constexpr long long TicksPerTenthSecond = TicksPerMillisecond * 100;

private:
    long long ticks_;

public:
    int getDays() const { return (int)(ticks_ / TicksPerDay); }
    int getHours() const { return (int)((ticks_ / TicksPerHour) % 24); }
    int getMillis() const { return (int)((ticks_ / TicksPerMillisecond) % 1000); }
    int getMinutes() const { return (int)((ticks_ / TicksPerMinute) % 60); }
    int getSeconds() const { return (int)((ticks_ / TicksPerSecond) % 60); }
    long long getTicks() const { return ticks_; }

    long long getTotalMilliseconds();
    long long getTotalSeconds();

    std::string toString() const;
    std::string toString(const char* Format) const;

private:
    static long long timeToTicks(int hour, int minute, int second);

public:
    TimeSpan operator - (const TimeSpan& rhs) const;

public:
    TimeSpan(long long ticks)
    {
        ticks_ = ticks;
    }

    TimeSpan(int hours, int minutes, int seconds)
    {
        ticks_ = timeToTicks(hours, minutes, seconds);
    }
};