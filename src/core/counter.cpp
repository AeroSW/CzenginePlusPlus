#include "counter.hpp"

namespace CzaraEngine {
    Counter::Counter() : m_value(0) {}
    Counter::Counter(const i64 &init) : m_value(init) {}
    Counter::Counter(const Counter &counter) : m_value(counter.m_value) {};
    Counter::Counter(const Counter &&counter) : m_value(counter.m_value) {};
    Counter::Counter(Counter * counter) : m_value(counter->m_value) {}
    Counter::~Counter() {}
    // Get the value after incrementing (post incrementing).
    i64 Counter::postInc() {
        m_value++;
        return i64(m_value);
    }
    // Get value after decrimenting (post decrimenting)
    i64 Counter::postDec() {
        m_value--;
        return i64(m_value);
    }
    // Get value after before incrementing (pre-incremented)
    i64 Counter::preInc() {
        i64 response{m_value};
        m_value++;
        return response;
    }
    // Get value after before decrimenting (pre-decremented)
    i64 Counter::preDec() {
        i64 response{m_value};
        m_value--;
        return response;
    }
    i64 Counter::value() const {
        return i64(m_value);
    }
}