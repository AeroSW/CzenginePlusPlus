#include "exception.hpp"

#include <memory>
#include "app-logs.hpp"

namespace CzaraEngine {
    template<typename T, typename U>
    std::shared_ptr<U> static_pointer_cast(std::shared_ptr<T> &derived);
    template<typename T, typename U>
    std::shared_ptr<U> static_pointer_cast(const std::shared_ptr<T> &derived);

    //BEGIN Exception Wrappers
    void exceptionWrap(void (*func)()) {
        try {
            func();
        } catch (EngineException &engine_exception) {
            handleException(engine_exception);
        } catch (...) {
            Logger::err_log() << "An unknown exception occurred." << endl;
        }
    }
    template<typename... A>
    void exceptionWrapVoidWithArgs(void (*func)(A...), A... args) {
        try {
            func(args...);
        } catch (EngineException &engine_exception) {
            handleException(engine_exception);
        } catch (...) {
            Logger::err_log() << "An unknown exception occurred." << endl;
        }
    }
    template<typename T, typename... A>
    T exceptionWrapReturn(T (*func)(A...), A... args) {
        try {
            return func(args...);
        } catch (EngineException &engine_exception) {
            handleException(engine_exception);
        } catch (...) {
            Logger::err_log() << "An unknown exception occurred." << endl;
        }
    }
    //END Exception Wrappers

    void handleException(EngineException &engine_exception) {
            std::ostringstream stream;
            stream << engine_exception.msg;
            switch(engine_exception.exception_code) {
                case EngineExceptionCode::LOG_EXCEPTION:
                    stream << endl << "\tRoot logging exception detected!" << endl;
                case EngineExceptionCode::WINDOW_EXCEPTION:
                    stream << endl << "\tAn exception was detected initializing SDL2 Window!" << endl;
                    break;
                case EngineExceptionCode::MEMORY_EXCEPTION:
                    stream << endl << "\tAn exception was triggered by CzaraEngine Smart Pointer." << endl;
                    break;
            };
            Logger::err_log() << stream.str();
    }
}