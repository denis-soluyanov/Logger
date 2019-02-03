#pragma once

#include <string>
#include <ctime>

#if defined(_WIN32)
    #define gmtime_r(time, result)      gmtime_s((result), (time))
    #define localtime_r(time, result)   localtime_s((result), (time))
#endif


enum class Timezone
{
    UTC,    // use UTC time to display in log-files
    LOCAL   // use Local (system) time to display in log-files
};

std::string getCurrentTime(Timezone timezone)
{
    time_t  rawTime = 0;
    tm      convertedTime;
    char    formattedTime[80];

    time(&rawTime);

    if (timezone == Timezone::UTC)
        gmtime_r(&rawTime, &convertedTime);
    else if (timezone == Timezone::LOCAL)
        localtime_r(&rawTime, &convertedTime);

    std::strftime(formattedTime, sizeof(formattedTime), "[%F %T %z]:", &convertedTime);
    return formattedTime;
}

template<typename OutStream, typename T>
class FormatOutput final
{
private:
    const T& ref_;

public:
    FormatOutput(const T& obj) : ref_(obj) {}

    friend OutStream& operator<<(OutStream& os, const FormatOutput<OutStream, T>& arg)
    {
        os << arg.ref_ << ' ';
        return os;
    }
};


#undef gmtime_r
#undef localtime_r