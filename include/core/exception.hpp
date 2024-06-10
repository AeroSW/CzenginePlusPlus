#pragma once

#include <string>
#include <sstream>

namespace CzaraEngine {

    enum class EngineExceptionCode {
        LOG_EXCEPTION,
        WINDOW_EXCEPTION,
        MEMORY_EXCEPTION
    };
    struct EngineException {
        EngineExceptionCode exception_code;
        std::string msg;
    };
    const std::string err_log_name = "err_log";
    
    void exceptionWrap(void (*func)());
    template<typename... A>
    void exceptionWrapVoid(void (*func)(A...), A... args);
    template<typename T, typename... A>
    T exceptionWrapReturn(T (*func)(A...), A... args);

    void handleException(EngineException&);
    void setupErrLog();
    
    #ifndef THROW_EXCEPTION
        // err_code is an EngineExceptionCode
        #define THROW_EXCEPTION(err_code, msg) \
            std::ostringstream oss; \
            oss << __FILE__ << "::" << __LINE__ << msg; \
            EngineException exc = {err_code, oss.str()}; \
            throw exc;
    #endif
}