#pragma once
#include "inttypes.hpp"
#include "counter.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <ostream>
#include <memory>

namespace CzaraEngine {
    template<typename T>
    class Shared {
        public:
            Shared();
            Shared(T * ref);
            Shared(T * ref, Counter * counter);
            Shared(const Shared<T> &shared_ref);
            Shared(Shared<T> &&shared_ref);
            ~Shared();

            T * get();
            Counter * getCounterPtr();
            void set(T * ref);

            Shared<T>& operator=(const Shared<T> &shared_ref);
            Shared<T>& operator=(Shared<T> &&shared_ref);
            bool isNullptr();
            const T * operator->();
            const T& operator*();
            const ui64 count();
            std::ostream debug(const std::string &pre, const std::string &post);
            template<typename U>
            friend Shared<U> static_pointer_cast(Shared<T> derived) {
                derived.invalidStateCheck();
                U * log = (U*) derived.get();
                Shared<U> parent(log, derived.getCounterPtr());
                return parent;
            }
        private:
            Counter * counter;
            T * reference;
            void deref();
            void invalidStateCheck() const;
    };

    // Shared<T>
    template<typename T>
    Shared<T>::Shared() {
        counter = new Counter(); // Initializes counter to 0 by default
        reference = nullptr;
    }
    template<typename T>
    Shared<T>::Shared(T * ref) {
        if (ref == nullptr) {
            counter = new Counter();
        } else {
            counter = new Counter(1);
        }
        reference = ref;
    }
    template<typename T>
    Shared<T>::Shared(T * ref, Counter * counter) {
        if (counter == nullptr) {
            throw "Counter cannot be null in Shared<T>";
        }
        if (ref == nullptr && counter->value() > 0) {
            throw "Counter state exception.";
        }
        this->reference = ref;
        this->counter = counter;
        this->counter->postInc();
    }

    template<typename T>
    Shared<T>::Shared(const Shared<T> &shared_ref) {
        shared_ref.invalidStateCheck();
        this->reference = shared_ref.reference;
        this->counter = shared_ref.counter;
        if (this->reference != nullptr) {
            if (this->counter != nullptr && this->counter->value() >= 1) {
                this->counter->postInc();
            }
        }
    }

    template<typename T>
    Shared<T>::Shared(Shared<T> &&dying_ref) {
        dying_ref.invalidStateCheck();
        this->counter = dying_ref.counter; // Repoint to shared_ref's counter.
        this->reference = dying_ref.reference; // Repoint to shared_ref's reference.
        // No need to increment, since, dying_ref will no longer reference this instance of T.
    }

    template<typename T>
    Shared<T>::~Shared() {
        deref();
    }

    template<typename T>
    T * Shared<T>::get() {
        return this->reference;
    }
    template<typename T>
    Counter * Shared<T>::getCounterPtr() {
        return this->counter;
    }
    template<typename T>
    void Shared<T>::set(T * ref) {
        if (reference != nullptr) {
            deref();
        }
        reference = ref;
        if (ref != nullptr) {
            counter = new Counter(1);
        } else {
            counter = new Counter();
        }
    }

    template<typename T>
    Shared<T>& Shared<T>::operator=(const Shared<T> &shared_ref) {
        shared_ref.invalidStateCheck();
        deref();
        this->counter = shared_ref.counter; // Repoint to shared_ref's counter.
        this->reference = shared_ref.reference; // Repoint to shared_ref's reference.
        this->counter->postInc(); // Increment, since, we now have a new reference to shared_ref's reference.
        return *this;
    }

    template<typename T>
    Shared<T>& Shared<T>::operator=(Shared<T> &&dying_ref) {
        dying_ref.invalidStateCheck();
        deref();
        this->counter = dying_ref.counter; // Repoint to shared_ref's counter.
        this->reference = dying_ref.reference; // Repoint to shared_ref's reference.
        // No need to increment, since, dying_ref will no longer reference this instance of T.
        return *this;
    }

    template<typename T>
    bool Shared<T>::isNullptr() {
        return this->reference == nullptr;
    }

    template<typename T>
    const T * Shared<T>::operator->() {
        if (this->reference == nullptr) {
            throw "Unable to access members of nullptr.";
        }
        return this->reference;
    }

    template<typename T>
    const T& Shared<T>::operator*() {
        if (reference == nullptr) {
            throw "Unable to dereference a nullptr.";
        }
        return *this->reference;
    }

    template<typename T>
    const ui64 Shared<T>::count() {
        if (counter == nullptr) {
            throw "Counter is null while Shared<T> is alive.";
        }
        return counter->value();
    }

    template<typename T>
    void Shared<T>::deref() {
        if (counter != nullptr && counter->value() <= 0) {
            if (reference != nullptr) delete reference;
            delete counter;
            counter = nullptr;
            reference = nullptr;
        } else if (counter != nullptr && counter->value() > 0) {
            counter->postDec();
        } else {
            throw "Counter is null prior to deref call.";
        }
    }

    template<typename T>
    void Shared<T>::invalidStateCheck() const {
        if (this->counter == nullptr) {
            throw "Invalid counter state.  Counter should not be a nullptr.";
        }
        if (this->reference == nullptr && this->counter->value() != 0) {
            throw "Invalid counter state.  Counter is non-zero value when reference is null.";
        }
        if (this->reference != nullptr && this->counter->value() <= 0) {
            throw "Invalid counter state.  Counter is null or non-positive value when reference is NOT null.";
        }
    }

    template<typename T>
    std::ostream Shared<T>::debug(const std::string &pre, const std::string &post) {
        std::string active_count = (counter != nullptr) ? std::to_string(counter->value()) : "nullptr";
        std::string ref_value = (reference != nullptr) ? std::to_string(reference) : "nullptr";
        std::ostream output_stream;
        return output_stream << " Ref: " << ref_value << " Count: " << active_count << " | " << pre << " Shared<T> " << post;
    }
    // end Shared<T>
}
