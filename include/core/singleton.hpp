#pragma once

#include "counter.hpp"
#include "shared.hpp"
#include "exception.hpp"

namespace CzaraEngine {
    template<typename T>
    class Singleton {
        public:
            template<typename... A>
            bool initReference(A... args) {
                if (m_reference != nullptr) {
                    return false;
                }
                m_reference = new T(args...);
                return true;
            }
            virtual ~Singleton();
            bool hasReference();
            static Singleton<T>& getSingletonInstance();
            T& getReference();
        private:
            Singleton();
            static Singleton<T> m_instance;
            static T* m_reference;
    };
    template<typename T>
    Singleton<T> Singleton<T>::m_instance;
    template<typename T>
    T * Singleton<T>::m_reference = nullptr;

    template<typename T>
    Singleton<T>& Singleton<T>::getSingletonInstance() {
        return m_instance;
    }

    template<typename T>
    Singleton<T>::Singleton() {}
    template<typename T>
    Singleton<T>::~Singleton() {
        if (m_reference != nullptr) {
            delete m_reference;
            m_reference = nullptr;
        }
    }

    template<typename T>
    bool Singleton<T>::hasReference() {
        return m_reference != nullptr;
    }
    template<typename T>
    T& Singleton<T>::getReference() {
        if (m_reference == nullptr) {
            THROW_EXCEPTION(EngineExceptionCode::MEMORY_EXCEPTION, "m_reference should not be null.");
        }
        return *m_reference;
    }
}
