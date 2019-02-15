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

    struct Time
    {
        Timezone timezone  = Timezone::LOCAL;
        std::string format = "%F %T.000";
        bool showTimezone  = true;
    };

    struct Adapter
    {
        bool logTime             = true;         // Writes current time in log-files
        Time time;                               //
        bool printInConsole      = false;        // Also print message in Console
        uint64_t maxFileSize     = 0;            // Max size of log-file in bytes (if '0' - no limit)
        bool enableFileRotation  = false;        // Enable transfer log-file to backup-folder
        std::string backupFolder = "./backlogs"; // Folder for old log-files
    };

    inline std::string getCurrentTime(const Time& time)
    {
        using namespace std::chrono;

        auto currentTime = system_clock::now();
        auto rawTime = system_clock::to_time_t(currentTime);
        auto ms = std::to_string(duration_cast<milliseconds>(currentTime.time_since_epoch()).count() % 1000);

        tm convertedTime;
        char formattedTime[40];
        std::string format = time.format;

        format.replace(format.find_last_of('0') - ms.size() + 1, ms.size(), ms.c_str());

        if (time.timezone == Timezone::UTC) {
            gmtime_r(&rawTime, &convertedTime);
            if (time.showTimezone) format += " UTC";
        }
        else if (time.timezone == Timezone::LOCAL) {
            localtime_r(&rawTime, &convertedTime);
            if (time.showTimezone) format += " UTC%z";
        }

        strftime(formattedTime, sizeof(formattedTime), format.c_str(), &convertedTime);

        return formattedTime;
    }
}

namespace dlog {
inline namespace size_literals {
    constexpr uint64_t operator "" _B(unsigned long long val)
    {// returns number of bytes
        return val;
    }

    constexpr uint64_t operator "" _KB(unsigned long long val)
    {// takes number of kilobytes and converts it into bytes
        return (val * 1024);
    }

    constexpr uint64_t operator "" _MB(long double val)
    {// takes number of megabytes and converts it into bytes
        return static_cast<uint64_t>(val * 1024 * 1024);
    }

    constexpr uint64_t operator "" _GB(long double val)
    {// takes number of gigabytes and converts it into bytes
        return static_cast<uint64_t>(val * 1024 * 1024 * 1024);
    }
}
}

#undef gmtime_r
#undef localtime_r