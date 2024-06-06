#pragma once

#include <sstream>
#include <ostream>

namespace CzaraEngine {
    class Log {
        public:
        Log() {};
        virtual ~Log() {};
        virtual std::ostringstream getClassDetails() const = 0;
        virtual std::ostream& write(const char*) = 0;
        virtual std::ostream& write(const std::string&) = 0;
        virtual std::ostream& write(std::ostream &) = 0;
    };
    std::ostream& operator<<(Log &log, const char* msg);
    std::ostream& operator<<(Log &log, const std::string &msg);
    std::ostream& operator<<(Log &log, std::ostream &msg);
    std::ostream& operator<<(std::ostream &stream, const Log &log);
}
