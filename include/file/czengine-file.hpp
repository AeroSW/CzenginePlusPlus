#pragma once

#include <filesystem>

#include "shared.hpp"
#include "log-file.hpp"
#include "exception.hpp"
#include "log-manager.hpp"

namespace fs = std::filesystem;

namespace CzaraEngine {
    template<typename T>
    class ZcengineFile {
        public:
            virtual ~ZcengineFile();
            virtual T processFile() = 0;
        protected:
            ZcengineFile(const fs::path path);
            fs::path m_path;
            static LogManager m_log_manager;
            constexpr static m_log_name = "Zcengine_File_Log";
    };
    template<typename T>
    ZcengineFile<T>::ZcengineFile(const fs::path path) : m_path(path) {
        Shared<SizeLogFile> log{{10000, m_log_name, app_config.getReference().default_log_dir}, 4};
        m_log_manager.registerLog(m_log_name, static_pointer_cast<Log>(log));
    }
    template<typename T>
    ZcengineFile<T>::~ZcengineFile() {}
}
