#include "log.hpp"
#include <memory>
#include <iostream>

namespace CzaraEngine {
    std::ostream& operator<<(Log &log, const std::string &msg) {
        return log.write(msg);
    }
    std::ostream& operator<<(Log &log, const char* msg) {
        return log.write(msg);
    }
    std::ostream& operator<<(Log &log, std::ostream &stream) {
        return log.write(stream);
    }

    std::ostream& operator<<(std::ostream &stream, const Log& log) {
        std::ostringstream output_stream = log.getClassDetails();
        stream << output_stream.str();
        return stream;
    }
}
