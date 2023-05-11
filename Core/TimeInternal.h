#pragma once

class TimeInternal
{
public:
    static constexpr long long g_nanosecondsPerTick = 100; // 틱당 나노초, 1틱당 100나노초

    static constexpr long long g_ticksPerMicrosecond = 10; // 10틱당 1마이크로초
    static constexpr long long g_ticksPerMillisecond = 10000; // 10000틱당 1밀리초
    static constexpr long long g_ticksPerSecond = g_ticksPerMillisecond * 1000;
    static constexpr long long g_ticksPerMinute = g_ticksPerSecond * 60;
    static constexpr long long g_ticksPerHour = g_ticksPerMinute * 60;
    static constexpr long long g_ticksPerDay = g_ticksPerHour * 24;

    // Number of milliseconds per time unit
    static constexpr int g_millisPerSecond = 1000;
    static constexpr int g_millisPerMinute = g_millisPerSecond * 60;
    static constexpr int g_millisPerHour = g_millisPerMinute * 60;
    static constexpr int g_millisPerDay = g_millisPerHour * 24;

    static constexpr int g_daysPerYear = 365; // Number of days in a non-leap year
    static constexpr int g_daysPer4Years = g_daysPerYear * 4 + 1;        // 1461;
    static constexpr int g_daysPer100Years = g_daysPer4Years * 25 - 1;    // 36524
    static constexpr int g_daysPer400Years = g_daysPer100Years * 4 + 1; // 146097

    // Number of days from 1/1/0001 to 12/31/1600
    static constexpr int g_daysTo1601 = g_daysPer400Years * 4; // 584388
    // Number of days from 1/1/0001 to 12/31/1899
    static constexpr int g_daysTo1899 = g_daysPer400Years * 4 + g_daysPer100Years * 3 - 367;
    // Number of days from 1/1/0001 to 12/31/1969
    static constexpr int g_daysTo1970 = g_daysPer400Years * 4 + g_daysPer100Years * 3 + g_daysPer4Years * 17 + g_daysPerYear; // 719,162
    // Number of days from 1/1/0001 to 12/31/9999
    static constexpr int g_daysTo10000 = g_daysPer400Years * 25 - 366;  // 3652059

    static constexpr long long g_minTicks = 0;
    static constexpr long long g_maxTicks = g_daysTo10000 * g_ticksPerDay - 1;
    static constexpr long long g_maxMillis = (long long)g_daysTo10000 * g_millisPerDay;

    static constexpr long long g_fileTimeOffset = g_daysTo1601 * g_ticksPerDay;
    static constexpr long long g_doubleDateOffset = g_daysTo1899 * g_ticksPerDay;
    // The minimun OA date is 0100/01/01 (Note it's year 100)
    // The Maximun OA date is 9999/12/31
    static constexpr long long OADateMinAsTicks = (g_daysPer100Years - g_daysPerYear) * g_ticksPerDay;
    // All OA dates must be greater than (not >=) OADateMinAsDouble
    static constexpr double OADateMinAsDouble = -657435.0;
    // All OA dates must be less than (not <=) OADateMaxAsDouble
    static constexpr double OADateMaxAsDouble = 2958466.0;

    static constexpr int g_datePartYear = 0;
    static constexpr int g_datePartDayOfYear = 1;
    static constexpr int g_datePartMonth = 2;
    static constexpr int g_datePartDay = 3;

    // static constexpr bool s_isLeapSecondsSupportedSystem = SystemSupportLeapSeconds();

    static constexpr int g_daysToMonth365[] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365 };
    static constexpr int g_daysToMonth366[] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366 };

    //public static readonly DateTime MinValue = new DateTime(MinTicks, DateTimeKind.Unspecified);
    //public static readonly DateTime MaxValue = new DateTime(MaxTicks, DateTimeKind.Unspecified);

    static constexpr unsigned long long g_ticksMask = 0x3FFFFFFFFFFFFFFF;
    static constexpr unsigned long long g_flagsMask = 0xC000000000000000;
    static constexpr unsigned long long g_localMask = 0x8000000000000000;
    static constexpr long long g_ticksCeiling = 0x4000000000000000;
    static constexpr unsigned long long g_kindUnspecified = 0x0000000000000000;
    static constexpr unsigned long long g_kindUtc = 0x4000000000000000;
    static constexpr unsigned long long g_kindLocal = 0x8000000000000000;
    static constexpr unsigned long long g_kindLocalAmbiguousDst = 0xC000000000000000;
    static constexpr unsigned int g_kindShift = 62;

public:
    static long long dateToTicks(int year, int month, int day);
    static long long timeToTicks(int hour, int minute, int second, int millisecond);
    static long long millisecondToTicks(long long millisecond);
    static bool isLeapYear(int year);

    static long long assign(int Days, int Hours, int Minutes, int Seconds, int Millisecond, int Microsecond, int FractionNano);

    static long long internalTicks(unsigned long long dateData);
    static unsigned long long internalKind(unsigned long long dateData);
    static int getDatePart(unsigned long long dateData, int part);
    static void getDatePart(unsigned long long dateData, int& year, int& month, int& day);

    static int daysInYear(int Year);
    static int daysInMonth(int Year, int Month);
    static bool isValidate(int Year, int Month, int Day);
    static bool isValidate(int Year, int Month, int Day, int Hour, int Minute, int Second, int Millisecond);

    static unsigned long long add(unsigned long long dateData, double value, int scale);
    static unsigned long long addTicks(unsigned long long dateData, long long value);

};