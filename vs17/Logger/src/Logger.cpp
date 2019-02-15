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


    void Logger::init(const std::string& filename, uint64_t maxFileSize, bool printInConsole)
    {
        Adapter adapter;
        adapter.maxFileSize = maxFileSize;
        adapter.printInConsole = printInConsole;

        init(filename, adapter);
    }

    void Logger::init(const std::string& filename, const Adapter& adapter)
    {
        static std::once_flag isCreated;

        if (!Logger::instance_)
            std::call_once(isCreated, Logger::createInstance, filename, adapter);
        else
            throw std::logic_error("Attempt to create instance of Logger twice");
    }

    bool Logger::canWrite() noexcept
    {
        return (adapter_.maxFileSize != 0) && (getFileSize() <= adapter_.maxFileSize);
    }

    Logger::LoggerPtr Logger::instance_ = nullptr;
}