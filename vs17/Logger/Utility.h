#pragma once

#include <string>
#include <ctime>
#include <chrono>

#if defined(_WIN32)
    #define gmtime_r(time, result)      gmtime_s((result), (time))
    #define localtime_r(time, result)   localtime_s((result), (time))
#endif

namespace dlog
{
    enum class Timezone
    {
        UTC,    // use UTC time to display in log-files
        LOCAL   // use Local (system) time to display in log-files
    };

    struct Format
    {
        bool logTime = true;
        Timezone timezone = Timezone::UTC;
        bool printInConsole = true;
    };

    std::string getCurrentTime(Timezone timezone)
    {
        using namespace std::chrono;

        auto currentTime = system_clock::now();
        auto rawTime = system_clock::to_time_t(currentTime);
        auto ms = std::to_string(duration_cast<milliseconds>(currentTime.time_since_epoch()).count() % 1000);

        tm convertedTime;
        char formattedTime[40];
        std::string format = "%F %T.000 UTC";

        format.replace(format.find_last_of('0') - ms.size() + 1, ms.size(), ms.c_str());

        if (timezone == Timezone::UTC) {
            gmtime_r(&rawTime, &convertedTime);
        }
        else if (timezone == Timezone::LOCAL) {
            localtime_r(&rawTime, &convertedTime);
            format += "%z";
        }

        strftime(formattedTime, sizeof(formattedTime), format.c_str(), &convertedTime);

        return formattedTime;
    }
}

#undef gmtime_r
#undef localtime_r