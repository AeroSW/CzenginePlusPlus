#pragma once

#include <functional>
#include "Exception.hpp"

namespace CzaraEngine {
    template<typename T>
    class Wrapper {
        public:
            virtual ~Wrapper();
            virtual T* get() const;
        protected:
            Wrapper(std::function<void(T*)> fn);
            T * m_instance;
        private:
            std::function<void(T*)> m_destroy;
    };
    template<typename T>
    Wrapper<T>::Wrapper(std::function<void(T*)> fn) : m_destroy(fn) {
        if (fn == nullptr) {
            m_destroy = [](T * val) { delete val; };
        }
    }
    template<typename T>
    Wrapper<T>::~Wrapper() {
        if (m_instance != nullptr) {
            m_destroy(m_instance);
            m_instance = nullptr;
        }
    }
    template<typename T>
    T* Wrapper<T>::get() const {
        return m_instance;
    }
}