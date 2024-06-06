#pragma once

#include "Log.hpp"
#include <map>
#include <string>
#include "shared.hpp"

namespace CzaraEngine{
    class LogManager {
        public:
            LogManager();
            virtual ~LogManager();
            virtual void registerLog(const std::string &log_name, const Shared<Log> &log);
            virtual void writeToLog(const std::string &log_name, const std::string &msg);
            virtual bool hasLog(const std::string &log_name) const;
        private:
            inline static const std::string m_personal = "LogManager_personal";
            static std::map<std::string, Shared<Log>> logs;
    };
}
