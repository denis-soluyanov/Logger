#include "ConsoleLogger.h"

namespace dlog
{
    ConsoleLogger::ConsoleLogger(std::ostream& os) : ostream_(os) 
    {
    }

    void ConsoleLogger::print_(const std::string& record)
    {
        std::lock_guard<std::mutex> guard(mtx_);

        ostream_.write(record.c_str(), record.size());
    }
}

