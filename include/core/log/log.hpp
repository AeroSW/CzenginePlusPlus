#pragma once
#include "inttypes.hpp"
#include "shared.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <filesystem>

// enum LogState {
//     DEBUG,
//     UA,
//     PROD
// };

// class Log {
//     public:
//         virtual ~Log();
//         void addFile(const LogFileProps &props);
//         static Shared<Log> Create(const LogState &state);
//         static void setLogState(const LogState &state);
//     private:
//         Log();
//         std::map<std::string, Shared<LogFile>> log_file_map;
//         static Shared<Log> log;
//         static LogState log_state;
// };