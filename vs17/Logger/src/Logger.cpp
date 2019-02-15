#include "dlog/Logger.h"

namespace dlog
{
    Logger::Logger(const std::string& filename, const Adapter& adapter) : adapter_(adapter)
    {
        if (!openFile(filename) || !canWrite()) {
            // TODO: move current file to /backlogs or don't write anymore
        }
    }

    void Logger::createInstance(const std::string& filename, const Adapter& adapter)
    {
        instance_ = LoggerPtr(new Logger(filename, adapter));
    }

    bool Logger::canWrite() noexcept
    {
        return (adapter_.maxFileSize != 0) && (getFileSize() <= adapter_.maxFileSize);
    }

    Logger::LoggerPtr Logger::instance_ = nullptr;
}