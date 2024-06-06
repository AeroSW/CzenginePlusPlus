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
    
    template<typename T, typename... A>
    T exceptionWrap(T (*func)(A...), A... args);

    void setupErrLog();
    
    #ifndef THROW_EXCEPTION
        // err_code is an EngineExceptionCode
        #define THROW_EXCEPTION(err_code) \
            std::ostringstream oss; \
            oss << __FILE__ << " | " << __LINE__; \
            EngineException exc = {err_code, oss.str()};
    #endif
}