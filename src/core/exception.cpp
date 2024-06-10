#include "exception.hpp"

#include "shared.hpp"
#include "log-manager.hpp"
#include "log-file.hpp"
#include "app-configuration.hpp"

namespace CzaraEngine {
    template<typename T, typename U>
    Shared<U> static_pointer_cast(Shared<T> &derived);
    template<typename T, typename U>
    Shared<U> static_pointer_cast(const Shared<T> &derived);

    //BEGIN Exception Wrappers
    void exceptionWrap(void (*func)()) {
        try {
            func();
        } catch (EngineException &engine_exception) {
            handleException(engine_exception);
        } catch (...) {
            setupErrLog();
            LogManager log_manager;
            log_manager.writeToLog(err_log_name, "An unknown exception occurred.");
        }
    }
    template<typename... A>
    void exceptionWrapVoid(void (*func)(A...), A... args) {
        try {
            func(args...);
        } catch (EngineException &engine_exception) {
            handleException(engine_exception);
        } catch (...) {
            setupErrLog();
            LogManager log_manager;
            log_manager.writeToLog(err_log_name, "An unknown exception occurred.");
        }
    }
    template<typename T, typename... A>
    T exceptionWrapReturn(T (*func)(A...), A... args) {
        try {
            return func(args...);
        } catch (EngineException &engine_exception) {
            handleException(engine_exception);
        } catch (...) {
            setupErrLog();
            LogManager log_manager;
            log_manager.writeToLog(err_log_name, "An unknown exception occurred.");
        }
    }
    //END Exception Wrappers

    void handleException(EngineException &engine_exception) {
            setupErrLog();
            LogManager log_manager;
            std::ostringstream stream;
            stream << engine_exception.msg;
            switch(engine_exception.exception_code) {
                case EngineExceptionCode::LOG_EXCEPTION:
                    stream << LogFile::endLine() << "\tRoot logging exception detected!";
                case EngineExceptionCode::WINDOW_EXCEPTION:
                    stream << LogFile::endLine() << "\tAn exception was detected initializing SDL2 Window!";
                    break;
                case EngineExceptionCode::MEMORY_EXCEPTION:
                    stream << LogFile::endLine() << "\tAn exception was triggered by CzaraEngine Smart Pointer.";
                    break;
            };
            log_manager.writeToLog(err_log_name, stream.str());
    }

    void setupErrLog() {
        const LogManager log_manager;
        std::string default_dir = ".";
        try {
            default_dir = app_config.getReference().default_log_dir;
        } catch (...) {
            // Catching prematurely.  Hopefully doesn't happen, since, int main() initializes app_config.
            std::cerr << "Critical system exception" << std::endl;
            std::exit(1);
        }
        const LogFileProps props = {
            3600,
            err_log_name,
            default_dir
        };
        if (!log_manager.hasLog(err_log_name)) {
            const Shared<TimeLogFile> err_log{new TimeLogFile(props)};
            LogManager log_manager;
            log_manager.registerLog(err_log_name, static_pointer_cast<Log>(err_log));
        }
    }
}