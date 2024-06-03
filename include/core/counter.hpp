#pragma once

#include "inttypes.hpp"

namespace CzaraEngine {
    class Counter {
        public:
            Counter();
            Counter(const i64 &init);
            Counter(const Counter &counter);
            Counter(const Counter &&counter);
            Counter(Counter * counter);
            virtual ~Counter();
            i64 preInc();
            i64 preDec();
            i64 postInc();
            i64 postDec();
            i64 value() const;
        private:
            i64 m_value;
    };
}
