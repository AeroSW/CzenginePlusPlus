// #include "log.hpp"
// #include <sstream>



// // Log
// Log::Log() {}
// Log::~Log() {}
// Shared<Log> Log::Create(const LogState &state) {
//     Log::log_state = state;
//     if (log.get() == nullptr) {
//         log = Shared(new Log());
//     }
//     return log;
// }
// void Log::addFile(const LogFileProps &props) {
//     Shared log_file{new LogFile(props)};
//     log_file_map[props.base_name] = log_file;
// }
// void Log::setLogState(const LogState &state) {
//     Log::log_state = state;
// }
// // End of Log
