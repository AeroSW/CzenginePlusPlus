#pragma once
#include "inttypes.hpp"
#include "shared.hpp"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <ostream>

namespace fs = std::filesystem;
namespace chrono = std::chrono;
namespace chrono_lit = std::chrono_literals;
using namespace chrono_lit;

namespace CzaraEngine {

    struct LogFileProps {
        ui64 amount;
        std::string base_name;
        std::string base_directory;
    };

    class LogFile {
        public:
            virtual ~LogFile();
            virtual bool isOpen() const;
            virtual bool close();
            friend bool open(const LogFile &log_file);

            std::ostringstream getClassDetails() const;
            virtual std::ostream& write(const char*) = 0;
            virtual std::ostream& write(std::ostream &) = 0;

        protected:
            LogFile(const LogFileProps &props);
            std::ofstream m_file;
            fs::path m_active_file;
            fs::path m_base_directory;
            std::string m_base_name;
            ui64 m_amount = 0;
            std::vector<fs::path> m_related_files;
            std::string m_line_end;
            virtual void rotateFile();
        private:
            virtual bool open(const fs::path &filename) = 0;
            virtual bool hasExceededAmount() const = 0;
            virtual fs::path generateFileName() const = 0;
    };
    std::ostream& operator<<(LogFile &log_file, const char* msg);
    std::ostream& operator<<(LogFile &log_file, std::ostream &msg);
    std::ostream& operator<<(std::ostream &stream, const LogFile &log_file);

    class TimeLogFile : public LogFile {
        public:
            TimeLogFile(const LogFileProps &log_file_props);
            virtual ~TimeLogFile();
            virtual std::ostream& write(const char* msg);
            virtual std::ostream& write(std::ostream &msg);
            //virtual std::ostream& operator<<(std::ostream &msg);
        private:
            chrono::time_point<chrono::system_clock> timestamp;
            constexpr static chrono::minutes min_time_allowed_in_minutes = 10min; // 10min
        private:
            virtual bool open(const fs::path &filename);
            virtual fs::path generateFileName() const;
            virtual bool hasExceededAmount() const;
    };

    class SizeLogFile : public LogFile {
        public:
            SizeLogFile(const LogFileProps &log_file_props, ui8 suffix_length);
            virtual ~SizeLogFile();
            virtual std::ostream& write(const char* msg);
            virtual std::ostream& write(std::ostream &msg);
            //virtual std::ostream& operator<<(std::ostream &msg);
        private:
            ui8 m_suffix_length;
            ui64 m_increment = 1;
            ui64 m_input_buffer = 0;
            const static ui64 min_bytes_allowed_in_file = 5000; // 5kB
        private:
            virtual bool open(const fs::path &filename);
            virtual fs::path generateFileName() const;
            virtual bool hasExceededAmount() const;
    };
}
