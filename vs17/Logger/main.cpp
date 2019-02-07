#include <iostream>
#include "Logger.h"

int main()
{
    dlog::Format format;
    format.logTime = true;
    format.printInConsole = false;
    format.timezone = dlog::Timezone::LOCAL;

    dlog::Logger::init("test2.log", format);

    for (auto i = 0; i < 100; ++i) {
        dlog::Logger::log("Hello World! i =", i);
    }

    return 0;
}